"""PNG -> 32bit BMP(premultiplied alpha) 변환기.

AlphaBlend(AC_SRC_ALPHA)로 그릴 수 있도록 알파 채널을 유지하고,
R/G/B에 알파를 미리 곱해서 저장한다. (표준 라이브러리만 사용)

사용법:
    python png2bmp32.py 파일.png [파일2.png | 폴더 ...] [-o 출력폴더] [-s 접미사]

    - 폴더를 주면 그 안의 *.png를 모두 변환한다. (하위 폴더 제외)
    - -o 를 생략하면 원본 PNG와 같은 폴더에 저장한다.
    - 출력 파일명은 <원본이름><접미사>.bmp (기본 접미사: _a)
    - 지원: 8/16bit RGB, RGBA, Gray, Gray+Alpha / 1~8bit 팔레트 (비인터레이스)
"""
import argparse
import os
import struct
import sys
import zlib

# 색 타입별 채널 수
CHANNELS = {0: 1, 2: 3, 3: 1, 4: 2, 6: 4}


def read_png(path):
    """PNG를 읽어 (w, h, [행마다 RGBA bytearray]) 를 돌려준다."""
    with open(path, 'rb') as f:
        data = f.read()
    if data[:8] != b'\x89PNG\r\n\x1a\n':
        raise ValueError('PNG 파일이 아님')

    idat = b''
    plte = b''
    trns = b''
    i = 8
    while i < len(data):
        length, ctype = struct.unpack('>I4s', data[i:i + 8])
        body = data[i + 8:i + 8 + length]
        if ctype == b'IHDR':
            w, h, bit_depth, color_type, _, _, interlace = struct.unpack('>IIBBBBB', body)
        elif ctype == b'PLTE':
            plte = body
        elif ctype == b'tRNS':
            trns = body
        elif ctype == b'IDAT':
            idat += body
        i += 12 + length

    if interlace:
        raise ValueError('인터레이스 PNG는 지원하지 않음')
    if color_type not in CHANNELS:
        raise ValueError(f'지원하지 않는 색 타입: {color_type}')

    channels = CHANNELS[color_type]
    bits_per_pixel = channels * bit_depth
    bpp = max(1, bits_per_pixel // 8)              # 필터 단위(바이트)
    stride = (w * bits_per_pixel + 7) // 8
    raw = zlib.decompress(idat)

    # 필터 해제
    rows = []
    prev = bytearray(stride)
    p = 0
    for _ in range(h):
        ftype = raw[p]
        line = bytearray(raw[p + 1:p + 1 + stride])
        p += 1 + stride
        for x in range(stride):
            a = line[x - bpp] if x >= bpp else 0
            b = prev[x]
            c = prev[x - bpp] if x >= bpp else 0
            if ftype == 1:
                line[x] = (line[x] + a) & 255
            elif ftype == 2:
                line[x] = (line[x] + b) & 255
            elif ftype == 3:
                line[x] = (line[x] + ((a + b) >> 1)) & 255
            elif ftype == 4:
                pa, pb, pc = abs(b - c), abs(a - c), abs(a + b - 2 * c)
                pr = a if pa <= pb and pa <= pc else (b if pb <= pc else c)
                line[x] = (line[x] + pr) & 255
        rows.append(line)
        prev = line

    # 픽셀 -> RGBA(8bit)
    def sample(line, idx):
        """idx번째 샘플(채널 값)을 0~255 로 돌려준다. (16bit는 상위 바이트)"""
        if bit_depth == 8:
            return line[idx]
        if bit_depth == 16:
            return line[idx * 2]
        per_byte = 8 // bit_depth
        shift = 8 - bit_depth * (idx % per_byte + 1)
        return (line[idx // per_byte] >> shift) & ((1 << bit_depth) - 1)

    out_rows = []
    for line in rows:
        px = bytearray(w * 4)
        for x in range(w):
            if color_type == 6:
                r, g, b, a = (sample(line, x * 4 + k) for k in range(4))
            elif color_type == 2:
                r, g, b = (sample(line, x * 3 + k) for k in range(3))
                a = 255
            elif color_type == 0:
                v = sample(line, x)
                if bit_depth < 8:
                    v = v * 255 // ((1 << bit_depth) - 1)
                r = g = b = v
                a = 255
            elif color_type == 4:
                r = g = b = sample(line, x * 2)
                a = sample(line, x * 2 + 1)
            else:  # 3: 팔레트
                idx = sample(line, x)
                r, g, b = plte[idx * 3:idx * 3 + 3]
                a = trns[idx] if idx < len(trns) else 255
            px[x * 4:x * 4 + 4] = bytes((r, g, b, a))
        out_rows.append(px)
    return w, h, out_rows


def write_bmp32(path, w, h, rows):
    """RGBA 행들을 premultiply 해서 32bit BMP(BGRA, 아래->위)로 저장한다."""
    out = bytearray()
    for y in range(h - 1, -1, -1):
        r = rows[y]
        for x in range(w):
            R, G, B, A = r[x * 4:x * 4 + 4]
            out += bytes(((B * A + 127) // 255, (G * A + 127) // 255, (R * A + 127) // 255, A))
    header = b'BM' + struct.pack('<IHHI', 54 + len(out), 0, 0, 54)
    dib = struct.pack('<IiiHHIIiiII', 40, w, h, 1, 32, 0, len(out), 2835, 2835, 0, 0)
    with open(path, 'wb') as f:
        f.write(header + dib + out)


def collect(inputs):
    files = []
    for item in inputs:
        if os.path.isdir(item):
            files += [os.path.join(item, n) for n in sorted(os.listdir(item))
                      if n.lower().endswith('.png')]
        else:
            files.append(item)
    return files


def main():
    ap = argparse.ArgumentParser(description='PNG -> 32bit BMP(premultiplied alpha)')
    ap.add_argument('inputs', nargs='+', help='PNG 파일 또는 폴더')
    ap.add_argument('-o', '--out', help='출력 폴더 (기본: 원본과 같은 폴더)')
    ap.add_argument('-s', '--suffix', default='_a', help='출력 파일명 접미사 (기본: _a)')
    args = ap.parse_args()

    files = collect(args.inputs)
    if not files:
        print('변환할 PNG가 없습니다.')
        return 1
    if args.out:
        os.makedirs(args.out, exist_ok=True)

    fail = 0
    for src in files:
        name = os.path.splitext(os.path.basename(src))[0] + args.suffix + '.bmp'
        dst = os.path.join(args.out or os.path.dirname(os.path.abspath(src)), name)
        try:
            w, h, rows = read_png(src)
            write_bmp32(dst, w, h, rows)
            print(f'OK   {src} -> {dst} ({w}x{h})')
        except Exception as e:
            fail += 1
            print(f'FAIL {src}: {e}')
    return 1 if fail else 0


if __name__ == '__main__':
    sys.exit(main())

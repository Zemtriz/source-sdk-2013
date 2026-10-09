#!/usr/bin/env python3
"""Fetch DXVK's 64-bit d3d9.dll into game/bin/x64/dxvk/ for the mod launcher.

The launcher (src/launcher_main/main.cpp, LoadBundledDXVK) preloads that DLL
before the engine so Direct3D 9 runs on Vulkan. See docs/dxvk.md.

Usage:
    python fetch_dxvk.py                       # download the pinned release
    python fetch_dxvk.py --version 2.7.1       # download another release
    python fetch_dxvk.py --archive dxvk.tar.gz # use a tarball you already have
    python fetch_dxvk.py --sha256 <hex>        # also verify the tarball
"""

import argparse
import hashlib
import io
import pathlib
import sys
import tarfile
import urllib.request

DXVK_VERSION = "2.7.1"
DXVK_URL = "https://github.com/doitsujin/dxvk/releases/download/v{0}/dxvk-{0}.tar.gz"

ROOT = pathlib.Path(__file__).resolve().parents[2]
OUT_DIR = ROOT / "game" / "bin" / "x64" / "dxvk"

NOTICE = """DXVK {version} d3d9.dll (x64), loaded by the mod launcher before the engine.
Source: https://github.com/doitsujin/dxvk/releases/tag/v{version}
SHA-256 of the release tarball: {sha256}
DXVK is distributed under the zlib/libpng license:
https://github.com/doitsujin/dxvk/blob/master/LICENSE
Delete this folder, or launch with -nodxvk, to use native Direct3D 9.
"""


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--version", default=DXVK_VERSION)
    parser.add_argument("--archive", type=pathlib.Path, help="local dxvk-<version>.tar.gz to extract instead of downloading")
    parser.add_argument("--sha256", help="expected SHA-256 of the tarball")
    args = parser.parse_args()

    if args.archive:
        data = args.archive.read_bytes()
    else:
        url = DXVK_URL.format(args.version)
        print(f"Downloading {url}")
        with urllib.request.urlopen(url) as response:
            data = response.read()

    sha256 = hashlib.sha256(data).hexdigest()
    if args.sha256 and sha256 != args.sha256.lower():
        sys.exit(f"SHA-256 mismatch: got {sha256}, expected {args.sha256}")

    with tarfile.open(fileobj=io.BytesIO(data), mode="r:gz") as tar:
        member = next((m for m in tar.getmembers() if m.isfile() and m.name.endswith("/x64/d3d9.dll")), None)
        if member is None:
            sys.exit("x64/d3d9.dll not found in the archive")
        dll = tar.extractfile(member).read()

    OUT_DIR.mkdir(parents=True, exist_ok=True)
    (OUT_DIR / "d3d9.dll").write_bytes(dll)
    (OUT_DIR / "DXVK.txt").write_text(NOTICE.format(version=args.version, sha256=sha256))
    print(f"Wrote {OUT_DIR / 'd3d9.dll'} (tarball sha256 {sha256})")


if __name__ == "__main__":
    main()

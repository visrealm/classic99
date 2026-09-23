399.092

Open source (but restrictive license) emulator including ROMs licensed by Texas Instruments - see [documentation](https://github.com/tursilion/classic99/raw/main/dist/Classic99%20Manual.pdf) for license and restrictions.

Tips and tricks video: [https://www.youtube.com/watch?v=6Zok8TZLIP8](https://www.youtube.com/watch?v=6Zok8TZLIP8)

## Building

Visual Studio, and nothing else.

[pico9918-core](https://github.com/visrealm/pico9918-core) is MIT licensed and its
sources are in the tree under `3rdparty/pico9918-core`, compiled by
`classic99.vcxproj` alongside Classic99's own. `UPSTREAM.txt` there records the
revision they came from.

To move the library to a new revision, `3rdparty/update-pico9918-core.py` fetches it,
replaces those sources, and regenerates the headers and image arrays committed under
`3rdparty/pico9918-core-gen`. Build, then commit both together.

```
python 3rdparty/update-pico9918-core.py --fetch
python 3rdparty/update-pico9918-core.py --fetch 1944f45a5f945f66c57e73e624f9ca2861fa312e
python 3rdparty/update-pico9918-core.py --check
```

With no revision `--fetch` takes `main`; a commit needs its whole 40-character SHA.
`--check` writes nothing and reports whether the committed generated files still
match the sources. Python 3.8+, and git for `--fetch`.

The script refuses rather than guessing - a library option it does not recognise is
the usual one, and it says what to add where.

## VDP engine

Video -> Engine picks between Classic99's own VDP and
[pico9918-core](https://github.com/visrealm/pico9918-core), the emulation the
PICO9918 firmware runs; Video -> VDP picks the chip it answers as. Both apply on
the next reset. TMS9918 (pre-A), PICO9918 and PICO9918 PRO are core-only, so they
grey out under the Classic99 engine.

The core scans out a whole 640x480 VGA frame the way the board does - borders,
blanking, the faux CRT scanline effect and the F18A power-on badge are all part
of the picture - so the 80 column hack and the 128k hack do not apply to it, and
the F18A GPU runs inside the core rather than as a second CPU in the Classic99
debugger. The video filters are built around Classic99's own 272x208 buffer,
which that frame does not fit, so they grey out while the core is engaged; the
pick is restored when the Classic99 VDP comes back.

As a PICO9918 the board's 256-byte settings block is kept in `PICO9918.cfg`
beside `classic99.ini`, so the Configurator cartridge has something to configure
and its saves survive a restart. Delete the file to return to defaults.

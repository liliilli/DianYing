# `Dy` FontAtlasGenerator (v190301)

![_01](.\Docs\Readme\_01.png)

`Dy` Font Atlas generator is a font texture generator for `Dy` game-framework based on Qt and OpenGL.

You can create [SDF (Signed distance field)](https://en.wikipedia.org/wiki/Signed_distance_function) texture atlas given UTF-16 Character map codes with aribtary `.ttf` font files now.

Generated atlas information and `.png` texture altas would be used on `Dy` game framework.

## Features

This tool supports below features.

* 1K Resolution (1024x1024) SDF Font texture atlas generation.
* Setting two fonts (Secondary font is optional) to create texture atlas.
  * Note that global setting (Height of font, etc) is following the setting of Primary Font.
* Supporting default font code map, English, Hangul (한글), Kana (ひらがな・カタカナ) + Japanese Symbols, and CJK Hanbun (漢文)
  * Note that CJK Hanbun maps about 30000 glpyhs, so do not recommend just add CJK Hanbun maps into atlas. But recommend exporting atlas using dynamic glyph creation features as adding Text File.
* Dynamic font glyph creation using text file.
  * Detecting text glyphs of text file will be regarded as file is encoded as UTF-8.
* Export as compressed `dyFont` file that has compressed glyph information and glyph texture informations.
* Export as plain `json` file and `.png` texture atlas files separately.

## Structure

### Plain Json File Structure

Plain (Decompressed) Json file consists of two header category, `Character` and `Meta`. 

* `Character` Header consists of glyph informations of exported texture atlas files.

``` json
"Characters": {
  "96": {
    "HoriAdvance": 2.0, "HoriBearing": { "X": 36.875, "Y": 196.125 },
    "Scale": { "X": 54.179893493652344, "Y": 54.179893493652344 },
    "Size": { "X": 47.25, "Y": 46.0 },
    "TexCoordBox": { 
      "Channel": 0, "LeftDown": { "X": 0.5, "Y": 0.875 },
      "MapIndex": 0, "RightUp": { "X": 0.5625, "Y": 0.9375 }
    },
    "Translate": { "X": -5.675000190734863, "Y": -33.83124923706055 }
  },
  // ... and other glyph informationss
}
```

![](.\README.assets\metrics.png)

* `Size` is the glyph's width and height that expressed in font units (1pixel/64unit)
* `HoriBearing` is Left side bearing for horizontal layout in font units.
* `HoriAdvance` is Advance width for horizontal layout.
* `Scale` is font glyphs scaling information. When use this glyph in renderer, must descale this glyph using this values.
* `Translate` is position offset to render glyph properly. This value is scaled by 16 default for precision.
* `TexCoordBox` is rendering information how to find glyph texture from given texture altas and what texture coordinate to render.
  * `Channel` is used to find proper texture atlas, as `z`.
  * `MapIndex` is used to find channel that has proper atlas, `RGBA` is `0123`.
  * and two `LeftDown` and `RightUp` is texture coordinate for rendering. `D3D` is need to be inverted because different texture coordinates.

---

* `Meta` Header consists of Font name (Primary font name will be added), style specifier, Line feed, and UUID.

``` json
"Meta": {
	"FontSpecifierName": "MS Gothic",
	"FontStyleSpecifier": "Regular",
	"HoriLinefeed": 256,
	"Uuid": "028d5828-fc1f-464b-b6ff-06bac63bda1a"
}
```

`HoriLineFeed` is used to line feeding when positioning.

### Compressed File Structure (Only glyph information)

When exporting with compressed glyph information but leave texture files as `.png`, exported glpyh information file will have file specifier `.dyFntRes`. 

This file is compressed with `zlib` library and has below structure.

| Decompressed Buffer Length | Compressed Buffer Length (CBL) | Binary Buffer |
| -------------------------- | ------------------------------ | ------------- |
| 8 bytes                    | 8 bytes                        | CBL bytes     |

### Compressed File Structure (with Texture)

> in fix...

## Process

| Task            | Description                                                  | Percentage |
| --------------- | ------------------------------------------------------------ | ---------- |
| DynamicCreating | Read text file and select all characters from text file as UTF-16, so dynamically creating texture atlas. | 100%       |
| About           | Creating `About` window, populating and collapsing window.   | 50%        |
| ExportBinary    | Export created information and texture buffers as independent file which can be only read on `Dy`. | 50%        |
| MultipleFont    | Create information with multiple font and multiple independent options. | 100%       |
| Concurrent      | Detach creation process from UI thread.                      | 100%       |

* `ExportBinary` : Create information and image buffer file as `.dyFont` type specifier. <br>Need to implement importing dyFont file to `Dy` core. <br>Need to implement validation using SHA-2 (...and every `dy` file type.)
* TODO : remove FT_LOAD_NO_SCALE and change it to 26.6 fractional pixel format to unify.

## Third-party libraries

* zlib
* boost
* nlohmann json for modern c++
* Qt as GUI interface

## Copyright

This tool uses Qt open-source edition library as dynamic library.

https://doc.qt.io/qt-5/lgpl.html
# FiveM Offset Dumper

A lightweight, fast, and standalone offset dumper for FiveM.

## Usage

1. Launch **FiveM** and wait until you are fully loaded into the main menu or a server.
2. Run `FiveM.OffsetDumper.exe` as Administrator (required for `PROCESS_QUERY_INFORMATION` and memory reading).
3. The dumper will automatically attach to the target process (`FiveM_*_GTAProcess.exe`), scan for the defined signatures, and output the computed offsets.

## Building from Source

This project is built using C++ and requires the following to be compiled:

- **IDE**: Visual Studio 2022 (or newer)
- **Toolset**: MSVC v143
- **Configuration**: `Release` / `x64`

To compile, simply open `FiveM.OffsetDumper.sln` in Visual Studio and build the solution, or use MSBuild via the Developer Command Prompt:

```cmd
msbuild /p:Configuration=Release /p:Platform=x64 FiveM.OffsetDumper.sln
```

## Adding Custom Patterns

You can easily expand the offset definitions by adding new signatures to `memory/patterns.hpp`. 

```cpp
{"offset_name", "48 8B 05 ? ? ? ? 33 D2 48 8B 40 ...", true}, // Set to true if dereferencing is required
```

## Disclaimer

This project is provided strictly for **educational and research purposes**. Do not use this tool to violate terms of service or for malicious activities.

---

*Made by Ghost0Dev*

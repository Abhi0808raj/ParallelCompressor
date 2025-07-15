# Parallel File Compressor

A high-performance Qt6-based GUI application for compressing files using the Zstandard (Zstd) compression algorithm with multi-threading support.

## Features

- **Multi-threaded compression**: Utilizes multiple CPU cores for faster compression
- **Zstandard compression**: Uses the fast and efficient Zstd algorithm
- **User-friendly GUI**: Clean Qt6 interface with progress tracking
- **Configurable parameters**: Adjustable compression level and thread count
- **Real-time progress**: Visual progress bar showing compression status
- **Cross-platform**: Built with Qt6 for cross-platform compatibility

## Screenshot

The application provides a simple interface with:
- File selection for source and destination
- Compression level adjustment (1-22)
- Thread count configuration
- Real-time progress tracking

## Requirements

### Dependencies
- **Qt6**: Widgets module
- **Zstandard (libzstd)**: Compression library
- **CMake**: Build system (version 3.16 or higher)
- **C++17** compatible compiler
- **pkg-config**: For finding Zstd library

### System Requirements
- Linux/Unix-like system (tested configuration)
- Multi-core CPU (recommended for optimal performance)
- Sufficient RAM for file processing

## Installation

### Ubuntu/Debian
```bash
# Install dependencies
sudo apt update
sudo apt install qt6-base-dev libzstd-dev cmake build-essential pkg-config

# Clone and build
git clone <repository-url>
cd ParallelCompressor
mkdir build
cd build
cmake ..
make
```

### Fedora/RHEL
```bash
# Install dependencies
sudo dnf install qt6-qtbase-devel libzstd-devel cmake gcc-c++ pkgconfig

# Build
mkdir build
cd build
cmake ..
make
```

### Other Systems
Ensure you have Qt6 development libraries and libzstd installed, then follow the standard CMake build process.

## Usage

### GUI Application
1. Launch the application:
   ```bash
   ./ParallelCompressor
   ```

2. Select your source file using the "Browse..." button
3. Choose the destination file path
4. Adjust compression settings:
    - **Compression Level**: 1 (fastest) to 22 (best compression)
    - **Threads**: Number of CPU cores to use (default: auto-detected)
5. Click "Start Compression"
6. Monitor progress in the progress bar

### Compression Levels
- **1-3**: Fast compression, larger file size
- **4-9**: Balanced compression and speed
- **10-22**: Maximum compression, slower speed

### Threading
The application automatically detects your CPU core count and suggests an optimal thread count. You can manually adjust this based on your system's capabilities and current load.

## Technical Details

### Architecture
- **Main Thread**: Handles GUI and user interaction
- **Worker Thread**: Manages compression process coordination
- **Compression Threads**: Multiple threads for parallel chunk processing

### Compression Process
1. Input file is divided into 4MB chunks
2. Each chunk is compressed in parallel using available threads
3. Compressed chunks are written to output file in original order
4. Progress is reported throughout the process

### File Structure
```
src/
├── main.cpp              # Application entry point
├── MainWindow.cpp/.h     # Main GUI window
├── CompressionWorker.cpp/.h   # Compression logic
└── ThreadSafeQueue.cpp/.h     # Thread-safe data structures
```

## Configuration

### Build Options
The CMake configuration supports:
- Custom Qt6 installation paths
- Debug/Release builds
- Static/Dynamic linking options

### Runtime Configuration
- Compression level: 1-22 (adjustable via GUI)
- Thread count: 1-N (where N is your CPU core count)
- Chunk size: 4MB (compile-time constant)

## Performance Notes

### Optimal Settings
- **Thread Count**: Usually equal to your CPU core count
- **Compression Level**: 3-6 for balanced performance
- **Memory Usage**: Approximately `threads × chunk_size × 2` during compression

### Large Files
For very large files (>1GB), consider:
- Using fewer threads to reduce memory usage
- Ensuring sufficient free disk space (compressed size varies)
- Monitoring system resources during compression

## Troubleshooting

### Common Issues

**Build fails with Qt6 not found:**
```bash
# Set Qt6 path explicitly
export CMAKE_PREFIX_PATH=/path/to/qt6
cmake ..
```

**Zstd library not found:**
```bash
# Install development headers
sudo apt install libzstd-dev  # Ubuntu/Debian
sudo dnf install libzstd-devel # Fedora/RHEL
```

**Permission errors:**
- Ensure read access to source file
- Ensure write access to destination directory
- Check disk space availability

### Performance Issues
- Reduce thread count if system becomes unresponsive
- Lower compression level for faster processing
- Ensure sufficient RAM for large files

## Contributing

Contributions are welcome! Please consider:
- Code style consistency with existing codebase
- Adding unit tests for new features
- Updating documentation for changes
- Testing on multiple platforms

## License

[Add your license information here]

## Acknowledgments

- Built with [Qt6](https://www.qt.io/)
- Uses [Zstandard](https://facebook.github.io/zstd/) compression
- Inspired by modern parallel processing techniques

## Version History

- **v1.0**: Initial release with basic parallel compression
- Multi-threaded Zstd compression
- Qt6 GUI interface
- Progress tracking

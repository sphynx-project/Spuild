#!/bin/bash

SRC_DIR="src"
OBJ_DIR="build"
EXE="spuild"

CC="gcc"

CFLAGS="-Wall -Wextra -O2 -largp"

NUM_JOBS=$(nproc)
FORCE_REBUILD=0

print_help() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -B, --rebuild       Force rebuild of all source files."
    echo "  -c, --clean         Clean the build directory."
    echo "  -j, --jobs <N>      Set the number of parallel jobs (default: number of CPU cores)."
    echo "  -o, --output <FILE> Set the name of the output executable (default: spuild)."
    echo "  -h, --help          Display this help message and exit."
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case "$1" in
        -B|--rebuild)
            FORCE_REBUILD=1
            shift
            ;;
        -c|--clean)
            echo -e "\033[1;33mCleaning\033[0m $OBJ_DIR"
            rm -rf "$OBJ_DIR"/*
            echo -e "\033[1;32mClean complete!\033[0m"
            exit 0
            ;;
        -j|--jobs)
            if [[ -n "$2" && "$2" =~ ^[0-9]+$ ]]; then
                NUM_JOBS="$2"
                shift 2
            else
                echo "Error: --jobs requires a numerical argument."
                exit 1
            fi
            ;;
        -o|--output)
            if [[ -n "$2" ]]; then
                EXE="$2"
                shift 2
            else
                echo "Error: --output requires a filename."
                exit 1
            fi
            ;;
        -h|--help)
            print_help
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            print_help
            exit 1
            ;;
    esac
done

mkdir -p "$OBJ_DIR"

compile_source() {
    src_file="$1"
    obj_file="${OBJ_DIR}/$(basename "${src_file%.*}.o")"

    if [[ $FORCE_REBUILD -eq 0 && -f "$obj_file" && "$obj_file" -nt "$src_file" ]]; then
        echo -e "\033[1;36mSkipping\033[0m $src_file -> $obj_file (up to date)"
        return
    fi

    echo -e "\033[1;34mCompiling\033[0m $src_file -> $obj_file"
    if ! $CC $CFLAGS -c "$src_file" -o "$obj_file" 2>&1; then
        echo -e "\033[1;31mFailed to compile $src_file\033[0m"
        exit 1
    fi
}

PIDS=()

for src_file in $(find "$SRC_DIR" -type f -name "*.c"); do
    compile_source "$src_file" &
    PIDS+=($!)
    if (( ${#PIDS[@]} >= NUM_JOBS )); then
        wait -n
        PIDS=($(jobs -rp))
    fi
done

wait "${PIDS[@]}"

echo -e "\033[1;32mLinking\033[0m object files into $EXE"
OBJ_FILES=$(find "$OBJ_DIR" -type f -name "*.o")
if [ -n "$OBJ_FILES" ]; then
    $CC $OBJ_FILES -o "$EXE" 2>&1 | sed "s/^/  /"
    echo -e "\033[1;32mBuild complete!\033[0m Executable: $EXE"
else
    echo -e "\033[1;31mNo object files found for linking.\033[0m"
fi

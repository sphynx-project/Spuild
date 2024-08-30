#!/bin/bash

SRC_DIR="src"
OBJ_DIR="build"
EXE="spuild"
CC="gcc"
CFLAGS="-Wall -Wextra -O2"
NUM_JOBS=$(nproc)

mkdir -p "$OBJ_DIR"

compile_source() {
    src_file="$1"
    obj_file="${OBJ_DIR}/$(basename "${src_file%.*}.o")"
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

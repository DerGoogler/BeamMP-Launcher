get-content .env | foreach {
    $name, $value = $_.split('=')
    set-content env:\$name $value
}

echo "Using toochain from $env:VCPKG_TOOLCHAIN_PATH"
echo "Selected build type: $env:BUILD_TYPE"

echo "Prepare build"
cmake .\ -B .\bin -DCMAKE_BUILD_TYPE="$env:BUILD_TYPE" -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_TOOLCHAIN_PATH" -DVCPKG_TARGET_TRIPLET=x64-windows-static

echo "Started building"
cmake --build .\bin --config "$env:BUILD_TYPE"
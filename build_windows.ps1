# 强制UTF-8编码解决中文乱码
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
$OutputEncoding = [System.Text.Encoding]::UTF8

# 配置路径参数
$baseDir = $PSScriptRoot
Write-Host "项目根目录: $baseDir" -ForegroundColor Green

$buildDir = Join-Path -Path $baseDir -ChildPath "build/windows"
$installDir = Join-Path -Path $baseDir -ChildPath "dist/windows"

# 1. 创建必要的目录结构
$requiredDirs = @("build", "build/windows", "dist", "dist/windows")
foreach ($dir in $requiredDirs) {
    $fullPath = Join-Path -Path $baseDir -ChildPath $dir
    if (-not (Test-Path -Path $fullPath -PathType Container)) {
        Write-Host "创建目录: $fullPath" -ForegroundColor Cyan
        New-Item -ItemType Directory -Path $fullPath -Force | Out-Null
    }
}

# 2. 全量配置检测
$cmakeListsPath = Join-Path -Path $baseDir -ChildPath "CMakeLists.txt"
Write-Host "CmakeLists.txt: $cmakeListsPath" -ForegroundColor Green

if (-not (Test-Path -Path $cmakeListsPath -PathType Leaf)) {
    Write-Host "错误：在 $baseDir 未找到CMakeLists.txt" -ForegroundColor Red
    exit 1
}

# 3. 构建阶段
$buildSuccess = $true
$cmakeCache = Join-Path -Path $buildDir -ChildPath "CMakeCache.txt"
Write-Host "cmakeCache: $cmakeCache" -ForegroundColor Green

# 判断是否需要全量配置（首次构建或强制重建）
if (-not (Test-Path -Path $cmakeCache -PathType Leaf)) {
    Write-Host "未检测到CMake缓存，执行全量配置..." -ForegroundColor Yellow
    # 清空构建目录确保干净配置
    if (Test-Path -Path $buildDir -PathType Container) {
        Remove-Item -Path "$buildDir\*" -Recurse -Force
    }

    cmake -G "Visual Studio 17 2022" -A x64 -B "$buildDir" -S "$baseDir"

    if ($LASTEXITCODE -ne 0) {
        Write-Host "CMake配置失败！错误代码: $LASTEXITCODE" -ForegroundColor Red
        $buildSuccess = $false
    } else {
        Write-Host "全量配置完成，生成构建系统" -ForegroundColor Green
    }
} else {
    Write-Host "检测到CMake缓存，执行增量构建..." -ForegroundColor Cyan
    # 自动更新CMake配置（处理新增源文件等情况）
    cmake -B "$buildDir" -S "$baseDir"
    if ($LASTEXITCODE -ne 0) {
        Write-Host "CMake配置更新失败！错误代码: $LASTEXITCODE" -ForegroundColor Red
        $buildSuccess = $false
    }
}

# 4. 增量编译（仅在配置成功时执行）
if ($buildSuccess) {
    cmake --build "$buildDir" --config Release -j 32

    if ($LASTEXITCODE -ne 0) {
        Write-Host "编译失败！错误代码: $LASTEXITCODE" -ForegroundColor Red
        $buildSuccess = $false
    }
}

# 5. 安装阶段（仅在编译成功时执行）
if ($buildSuccess) {
    cmake --install "$buildDir" --config Release --prefix "$installDir"

    if ($LASTEXITCODE -ne 0) {
        Write-Host "安装失败！错误代码: $LASTEXITCODE" -ForegroundColor Red
    } else {
        Write-Host "编译安装完成！输出目录: $installDir" -ForegroundColor Green
    }
} else {
    Write-Host "编译失败，跳过安装步骤" -ForegroundColor Yellow
}
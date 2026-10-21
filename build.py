from pathlib import Path
import subprocess
import sys


PROJECT_ROOT = Path(__file__).resolve().parent
BUILD_DIR = PROJECT_ROOT / "build"


def run(command):
    print(">", " ".join(str(argument) for argument in command))
    sys.stdout.flush()
    subprocess.run(command, cwd=PROJECT_ROOT, check=True)


def application_path():
    candidates = [
        BUILD_DIR / "memdb_app",
        BUILD_DIR / "memdb_app.exe",
        BUILD_DIR / "Debug" / "memdb_app.exe",
        BUILD_DIR / "Release" / "memdb_app.exe",
    ]
    for candidate in candidates:
        if candidate.is_file():
            return candidate
    raise FileNotFoundError("memdb_app executable")


def main():
    command = sys.argv[1] if len(sys.argv) > 1 else "test"
    if command not in {"test", "run"}:
        print("Usage: python build.py [test|run] [capacity] [lru|fifo]",
              file=sys.stderr)
        return 2

    try:
        run(["cmake", "-S", ".", "-B", str(BUILD_DIR)])
        if command == "run":
            run([
                "cmake",
                "--build",
                str(BUILD_DIR),
                "--target",
                "memdb_app",
                "--parallel",
            ])
            application = application_path()
            run([str(application), *sys.argv[2:]])
        else:
            run(["cmake", "--build", str(BUILD_DIR), "--parallel"])
            run(["ctest", "--test-dir", str(BUILD_DIR), "--output-on-failure"])
    except FileNotFoundError as error:
        print(f"Required command not found: {error.filename}", file=sys.stderr)
        return 1
    except subprocess.CalledProcessError as error:
        return error.returncode

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

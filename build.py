from pathlib import Path
import subprocess
import sys


PROJECT_ROOT = Path(__file__).resolve().parent
BUILD_DIR = PROJECT_ROOT / "build"


def run(command):
    print(">", " ".join(str(argument) for argument in command))
    subprocess.run(command, cwd=PROJECT_ROOT, check=True)


def main():
    try:
        run(["cmake", "-S", ".", "-B", str(BUILD_DIR)])
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

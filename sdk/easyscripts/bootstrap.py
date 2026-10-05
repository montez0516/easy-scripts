import runpy
import sys
from pathlib import Path

script = Path(sys.argv[1]).resolve()
script_dir = script.parent

sdk_dir = Path(__file__).resolve().parent.parent

sys.path.insert(0, str(script_dir))
sys.path.insert(0, str(sdk_dir))

sys.argv = [
    str(script),
    *sys.argv[2:]
]

runpy.run_path(
    str(script),
    run_name="__main__"
)
import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

tools = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(tools))
import debt_fold

LINK = "../../../core/libs/prx/libSceExample/Export.cpp"
BASE = (f"# libSceExample technical debt\n\n### Silent stubs\n\n"
        f"- [sceExampleA]({LINK}) (libSceExample) - returns 0 for sizes \u2265 \u221e\n")
ADDED = f"- [sceExampleB]({LINK}) (libSceExample) - returns 1 \u2192 \u03c0\n"


class DebtFoldStdoutTests(unittest.TestCase):
    def test_show_writes_utf8_with_lf(self):
        with tempfile.TemporaryDirectory() as d:
            root = Path(d)
            debt = root / "docs/dev/debt/libSceExample.md"
            debt.parent.mkdir(parents=True)
            debt.write_bytes(BASE.encode("utf-8"))
            changes = root / "docs/dev/debt/changes"
            changes.mkdir()
            (changes / "a.md").write_bytes(f"# libSceExample\n\n## add Silent stubs\n\n{ADDED}".encode("utf-8"))
            env = {k: v for k, v in os.environ.items() if k not in ("PYTHONUTF8", "PYTHONIOENCODING")}
            result = subprocess.run(
                [sys.executable, str(tools / "debt_fold.py"), "--root", str(root), "--show", "libSceExample"],
                capture_output=True,
                env=env,
            )
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertNotIn(b"\r\n", result.stdout)
            self.assertEqual(result.stdout.decode("utf-8"), debt_fold.fold(root, write=False)[0]["libSceExample"])
            self.assertEqual(debt.read_bytes(), BASE.encode("utf-8"))


if __name__ == "__main__":
    unittest.main()

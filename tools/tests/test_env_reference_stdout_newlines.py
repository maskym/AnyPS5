from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

tools = Path(__file__).resolve().parents[1]


class EnvReferenceStdoutNewlineTests(unittest.TestCase):
    def run_env_reference(self, *extra, check=True):
        with tempfile.TemporaryDirectory() as d:
            root = Path(d)
            source = root / "core" / "libs" / "prx" / "libX" / "a.cpp"
            source.parent.mkdir(parents=True)
            source.write_bytes(b'const char* v = getenv("ANYPS5_X");\n')
            return subprocess.run(
                [sys.executable, str(tools / "env_reference.py"), "--root", str(root / "core")] + list(extra),
                capture_output=True,
                check=check,
            )

    def test_table_on_stdout_uses_lf(self):
        result = self.run_env_reference("--table")
        self.assertIn(b"\n", result.stdout)
        self.assertNotIn(b"\r\n", result.stdout)

    def test_listing_on_stdout_uses_lf(self):
        result = self.run_env_reference()
        self.assertIn(b"ANYPS5_X", result.stdout)
        self.assertIn(b"\n", result.stdout)
        self.assertNotIn(b"\r\n", result.stdout)

    def test_missing_root_error_on_stderr_uses_lf(self):
        with tempfile.TemporaryDirectory() as d:
            result = subprocess.run(
                [sys.executable, str(tools / "env_reference.py"), "--root", str(Path(d) / "missing")],
                capture_output=True,
            )
        self.assertEqual(result.returncode, 1)
        self.assertIn(b"\n", result.stderr)
        self.assertNotIn(b"\r\n", result.stderr)


if __name__ == "__main__":
    unittest.main()

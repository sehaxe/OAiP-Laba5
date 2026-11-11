#!/usr/bin/env python3
"""Прогоняет настоящие бинарники и складывает реальный вывод в assets/term.json.

Отчёт рисует из JSON терминальные окна вектором. Приглашения и наборы
ввода заданы явно: если приглашение не найдено или программа вернула
неожидаемый код возврата, генерация падает, чтобы рисунки не врали.
"""
import json
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent  # home/
ASSETS = ROOT / "report" / "assets"
TITLE = "sehaxe@cachyos: ~/OAiP-Laba5/home"

SIZE = "Размер массива n (1..100): "
ROWS = "Количество строк (1..20): "
MODE = "Заполнение: 1 - вручную, 2 - случайно: "
N = "Строк n (1..20): "
M = "Столбцов m (1..20): "
K = "K: "


def a_prompts(n: int) -> list[str]:
    return [SIZE] + [f"a[{i}] = " for i in range(n)]


def ragged_prompts(rows: list[int]) -> list[str]:
    out = [K, ROWS, MODE]
    for i, size in enumerate(rows):
        out += [f"a[{i}][{j}] (0 - конец): " for j in range(size + 1)]
    return out


def matrix_prompts(n: int, m: int) -> list[str]:
    return [N, M, MODE] + [f"a[{i}][{j}] = " for i in range(n) for j in range(m)]


SESSIONS = {
    1: [
        (a_prompts(6), ["6", "5", "3", "4", "2", "8", "7"]),
    ],
    2: [
        (ragged_prompts([3, 1, 0]),
         ["10", "3", "1", "5", "12", "3", "0", "20", "0", "0"]),
    ],
    3: [
        (matrix_prompts(3, 3), ["3", "3", "1", "1", "2", "3", "4", "5",
                                 "6", "7", "8", "9"]),
    ],
}

EXPECTED = {
    (1, 0): "Элементов осталось: 2",
    (2, 0): "Строка 1 (2):",
    (3, 0): "Седловая точка: a[0][2] = 3",
}


def screen_lines(prompts: list[str], user_input: str, out: str) -> list[dict]:
    inputs = user_input.split("\n") if user_input else []
    if len(inputs) != len(prompts):
        raise RuntimeError(f"вводов {len(inputs)}, приглашений {len(prompts)}")
    lines: list[dict] = []
    pos = 0
    for prompt, answer in zip(prompts, inputs):
        idx = out.find(prompt, pos)
        if idx < 0:
            raise RuntimeError(f"приглашение не найдено: {prompt!r}")
        for text in out[pos:idx].splitlines():
            if text:
                lines.append({"out": text})
        lines.append({"echo": {"prompt": prompt, "input": answer}})
        pos = idx + len(prompt)
    for text in out[pos:].splitlines():
        if text:
            lines.append({"out": text})
    return lines


def run_task(n: int, runs: list[tuple[list[str], list[str]]]) -> list[dict]:
    session = []
    for k, (run_prompts, inputs) in enumerate(runs):
        user_input = "\n".join(inputs)
        proc = subprocess.run(
            [f"./{n}/main"],
            input=user_input + "\n",
            capture_output=True,
            text=True,
            cwd=ROOT,
        )
        if proc.returncode != 0:
            raise RuntimeError(f"task {n}: код возврата {proc.returncode}")
        want = EXPECTED[(n, k)]
        if want not in proc.stdout:
            raise RuntimeError(f"task {n}, запуск {k}: нет {want!r}")
        session.append({
            "cmd": f"./{n}/main",
            "lines": screen_lines(run_prompts, user_input, proc.stdout),
        })
    return session


if __name__ == "__main__":
    ASSETS.mkdir(parents=True, exist_ok=True)
    data = {}
    for n, runs in SESSIONS.items():
        data[str(n)] = {
            "title": TITLE,
            "dir": "~/OAiP-Laba5/home",
            "runs": run_task(n, runs),
        }
        print(f"ok: задача {n} ({len(runs)} запуска)")
    out = ASSETS / "term.json"
    out.write_text(json.dumps(data, ensure_ascii=False, indent=2), encoding="utf-8")
    print(f"ok: {out.relative_to(ROOT)}")

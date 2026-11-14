"""Generate kernel/embed_apps.S from user/build/bin/*.bin.

Each app is embedded with `.incbin`. The `_num_app` table has the layout:
[num_app, app_0_start, app_1_start, ..., app_{n - 1}_start, app_{n - 1}_end]
"""

import os
import sys

USER_BIN_DIR = "user/build/bin"
OUTPUT = "kernel/embed_apps.S"
SRC_DIR = "user/build/bin"


def main() -> int:
    if not os.path.isdir(USER_BIN_DIR):
        print(f"[gen_apps] {USER_BIN_DIR} not found: writing empty table.")
        apps = []
    else:
        apps = sorted(name[:-2] for name in os.listdir(SRC_DIR) if name.endswith(".c"))

    for app in apps:
        bin_path = os.path.join(USER_BIN_DIR, f"{app}.bin")
        if not os.path.isfile(bin_path):
            print(f"[gen_apps] Missing {bin_path}.", file=sys.stderr)
            return 1

    with open(OUTPUT, "w") as f:
        print(
            "    .align 3\n    .section .data\n    .globl _num_app\n_num_app:", file=f
        )
        print(f"    .quad {len(apps)}", file=f)
        for app in apps:
            print(f"    .quad app_{app}_start", file=f)
        if len(apps) != 0:
            print(f"    .quad app_{apps[-1]}_end", file=f)
        print(file=f)

        for app in apps:
            print("    .section .data", file=f)
            print(f"    .globl app_{app}_start", file=f)
            print(f"    .globl app_{app}_end", file=f)
            print("    .align 3", file=f)
            print(f"app_{app}_start:", file=f)
            incbin_path = os.path.abspath(os.path.join(USER_BIN_DIR, f"{app}.bin"))
            print(f'    .incbin "{incbin_path}"', file=f)
            print(f"app_{app}_end:", file=f)
            print(file=f)

    print(f"[gen_apps] Embedded {len(apps)} app(s) into {OUTPUT}.")
    return 0


if __name__ == "__main__":
    sys.exit(main())

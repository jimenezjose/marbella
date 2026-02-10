import click
import questionary

@click.command()
def main():
    choice = questionary.select(
        "Select a module:",
        choices=[
            "Module A",
            "Module B",
            "Module C",
        ],
        use_shortcuts=True,
        style=questionary.Style([
            ("qmark", "fg:#ff9d00 bold"),
            ("question", "bold"),
            ("highlighted", "bg:#3a3a3a fg:#ffffff"),
            ("instruction", "italic fg:#888888"),
        ]),
        instruction="↑ ↓ to navigate • Enter to select • Ctrl-C to cancel",
    ).ask()


if __name__ == "__main__":
    main()

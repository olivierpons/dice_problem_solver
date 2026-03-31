"""UTF-8 symbol replacer for game entity names.

This module provides functionality to replace entity names
with their corresponding UTF-8 symbols in a text file.
"""

from __future__ import annotations

import argparse
import logging
import sys
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Sequence

from rich.console import Console
from tqdm import tqdm

logger = logging.getLogger(__name__)
console = Console()

SCRIPT_NAME: str = Path(__file__).name

SHORT_HELP: str = f"""\
Usage: python {SCRIPT_NAME} [OPTIONS]

Replace entity names with UTF-8 symbols in text files.

Options:
  -i, --input FILE       Input file (default: solutions.txt)
  -o, --output FILE      Output file (default: solutions_utf8.txt)
  -m, --mapping TEXT     Custom mapping "Name:symbol,Name2:symbol2"
  -s, --separator CHAR   Mapping separator (default: ,)
  -f, --force            Overwrite output file without confirmation
  -p, --progress         Show progress bar (default: enabled)
  --no-progress          Disable progress bar
  -v, --verbose          Verbose output
  -q, --quiet            Suppress non-error output
  -h                     Show this help message
  --help                 Show detailed help with examples

Examples:
  python {SCRIPT_NAME}
  python {SCRIPT_NAME} -i data.txt -o result.txt
  python {SCRIPT_NAME} -m "Hero:⚔️,Queen:👸" -s ","
  python {SCRIPT_NAME} --no-progress -q -f
"""


LONG_HELP: str = f"""\
UTF-8 Symbol Replacer - Detailed Help
=====================================

DESCRIPTION
-----------
This tool replaces entity names with their corresponding UTF-8 symbols
in text files. It processes lines containing entity definitions like
"Hero/3" and converts them to "⚔️/3".

DEFAULT ENTITY MAPPING
----------------------
  Hero      → ⚔️
  Captain   → 🎖️
  Peasant   → 👨‍🌾
  Queen     → 👸
  Soldier   → 🪖
  Potter    → ⚱️
  Scribe    → ✍️
  Mage      → 🧙
  Shaman    → 🪬
  Traitor   → 🗡️
  Thief     → 💰
  Cursed    → ☠️

OPTIONS
-------
  -i, --input FILE
      Input file path. Defaults to 'solutions.txt'.
      Example: -i myfile.txt

  -o, --output FILE
      Output file path. Defaults to 'solutions_utf8.txt'.
      Example: -o output.txt

  -m, --mapping TEXT
      Custom entity-to-symbol mapping. Format: "Name:symbol,Name2:symbol2"
      Overrides the default mapping completely.
      Example: -m "Warrior:⚔️,King:👑,Queen:👸"

  -s, --separator CHAR
      Character used to separate mappings in the --mapping option.
      Defaults to comma (,).
      Example: -m "Warrior:⚔️|King:👑" -s "|"

  -f, --force
      Force overwrite of existing output file without prompting.
      Without this flag, a confirmation is requested if the file exists.

  -p, --progress
      Enable progress bar (default). Shows processing progress with ETA.

  --no-progress
      Disable progress bar. Useful for scripting or when output is redirected.

  -v, --verbose
      Enable verbose logging. Shows debug information about operations.

  -q, --quiet
      Suppress all non-error output. Only errors are printed.

  -h
      Show short help message.

  --help
      Show this detailed help message.

EXAMPLES
--------
Basic usage with defaults:
  $ python {SCRIPT_NAME}
  → Reads solutions.txt, writes solutions_utf8.txt

Specify input and output:
  $ python {SCRIPT_NAME} -i data/input.txt -o results/output.txt

Custom mapping with default separator:
  $ python {SCRIPT_NAME} -m "Hero:⭐,Villain:👿"

Custom mapping with pipe separator:
  $ python {SCRIPT_NAME} -m "Hero:⭐|Villain:👿|NPC:🤖" -s "|"

Force overwrite existing file:
  $ python {SCRIPT_NAME} -o existing.txt -f

Process without progress bar (for scripts):
  $ python {SCRIPT_NAME} --no-progress -q

Verbose mode with custom files:
  $ python {SCRIPT_NAME} -v -i game.txt -o symbols.txt

Process multiple files in a loop:
  $ for f in *.txt; do python {SCRIPT_NAME} -i "$f" -o "{{f%.txt}}_utf8.txt" -q; done

INPUT FORMAT
------------
The tool expects lines in this format:
  [Hero/3, Captain/2, Queen/3] = 15 - [Soldier/1, Mage/4] = 5

Output will be:
  [⚔️/3, 🎖️/2, 👸/3] = 15 - [🪖/1, 🧙/4] = 5

ERROR HANDLING
--------------
The tool handles all common errors:
  - File not found
  - Permission denied (read/write)
  - Disk full / insufficient space
  - Encoding errors (UTF-8)
  - Invalid mapping format
  - Existing output file (with -f to override)

EXIT CODES
----------
  0  Success
  1  File not found or permission error
  2  Invalid arguments or mapping format
  3  Processing error (encoding, disk full, etc.)
  4  User cancelled (file exists, no -f flag)
"""


@dataclass(frozen=True)
class Config:
    """Application configuration.

    Attributes:
        input_path: Path to the input file.
        output_path: Path to the output file.
        mapping: Dictionary of entity names to symbols.
        force: Whether to overwrite existing files without confirmation.
        show_progress: Whether to display progress bar.
        quiet: Whether to suppress non-error output.
        verbose: Whether to enable verbose logging.
    """
    input_path: Path = field(default_factory=lambda: Path("solutions.txt"))
    output_path: Path = field(default_factory=lambda: Path("solutions_utf8.txt"))
    mapping: dict[str, str] = field(default_factory=dict)
    force: bool = False
    show_progress: bool = True
    quiet: bool = False
    verbose: bool = False

    def __post_init__(self) -> None:
        """Validate configuration after initialization."""
        if not self.mapping:
            object.__setattr__(
                self,
                "mapping",
                {
                    "Hero": "⚔️",
                    "Captain": "🎖️",
                    "Peasant": "👨‍🌾",
                    "Queen": "👸",
                    "Soldier": "🪖",
                    "Potter": "⚱️",
                    "Scribe": "✍️",
                    "Mage": "🧙",
                    "Shaman": "🪬",
                    "Traitor": "🗡️",
                    "Thief": "💰",
                    "Cursed": "☠️",
                },
            )


@dataclass(frozen=True)
class ProcessingResult:
    """Result of file processing.

    Attributes:
        success: Whether processing was successful.
        lines_processed: Number of lines processed.
        symbols_inserted: Number of symbols inserted.
        exit_code: Exit code for the operation.
        error_message: Error message if failed.
    """
    success: bool
    lines_processed: int = 0
    symbols_inserted: int = 0
    exit_code: int = 0
    error_message: str | None = None


class CustomArgumentParser(argparse.ArgumentParser):
    """Custom argument parser with rich-formatted help."""

    def print_help(self, file: Any | None = None) -> None:
        """Print short help message."""
        console.print(SHORT_HELP)

    def error(self, message: str) -> None:
        """Print error message and exit."""
        console.print(f"[red]Error:[/red] {message}")
        console.print(f"Use -h for help or --help for detailed help")
        sys.exit(2)

    def print_long_help(self) -> None:
        """Print detailed help message."""
        console.print(LONG_HELP)


def parse_mapping(mapping_str: str, separator: str) -> dict[str, str]:
    """Parse mapping string into dictionary.

    Args:
        mapping_str: String in format "Name:symbol,Name2:symbol2".
        separator: Character separating mappings.

    Returns:
        Dictionary of entity names to symbols.

    Raises:
        ValueError: If mapping format is invalid.
    """
    if not mapping_str:
        return {}

    result: dict[str, str] = {}
    pairs: list[str] = mapping_str.split(separator)

    for pair in pairs:
        pair = pair.strip()
        if not pair:
            continue

        if ":" not in pair:
            raise ValueError(
                f"Invalid mapping format: '{pair}'. Expected 'Name:symbol'"
            )

        name, symbol = pair.split(":", 1)
        name = name.strip()
        symbol = symbol.strip()

        if not name:
            raise ValueError("Entity name cannot be empty")
        if not symbol:
            raise ValueError(f"Symbol for '{name}' cannot be empty")

        result[name] = symbol

    return result


def create_parser() -> CustomArgumentParser:
    """Create and configure the argument parser.

    Returns:
        Configured argument parser with all options.
    """
    parser = CustomArgumentParser(
        prog=SCRIPT_NAME,
        add_help=False,
        description="Replace entity names with UTF-8 symbols",
    )

    parser.add_argument(
        "-i",
        "--input",
        type=str,
        default="solutions.txt",
        metavar="FILE",
        help="Input file path (default: solutions.txt)",
    )

    parser.add_argument(
        "-o",
        "--output",
        type=str,
        default="solutions_utf8.txt",
        metavar="FILE",
        help="Output file path (default: solutions_utf8.txt)",
    )

    parser.add_argument(
        "-m",
        "--mapping",
        type=str,
        default=None,
        metavar="TEXT",
        help='Custom mapping "Name:symbol,Name2:symbol2"',
    )

    parser.add_argument(
        "-s",
        "--separator",
        type=str,
        default=",",
        metavar="CHAR",
        help="Mapping separator character (default: ,)",
    )

    parser.add_argument(
        "-f",
        "--force",
        action="store_true",
        help="Overwrite output file without confirmation",
    )

    parser.add_argument(
        "-p",
        "--progress",
        action="store_true",
        default=True,
        dest="progress",
        help="Show progress bar (default: enabled)",
    )

    parser.add_argument(
        "--no-progress",
        action="store_false",
        dest="progress",
        help="Disable progress bar",
    )

    parser.add_argument(
        "-v",
        "--verbose",
        action="store_true",
        help="Enable verbose output",
    )

    parser.add_argument(
        "-q",
        "--quiet",
        action="store_true",
        help="Suppress non-error output",
    )

    parser.add_argument(
        "-h",
        action="store_true",
        dest="short_help",
        help="Show short help message",
    )

    parser.add_argument(
        "--help",
        action="store_true",
        dest="long_help",
        help="Show detailed help with examples",
    )

    return parser


def replace_entities_with_symbols(text: str, mapping: dict[str, str]) -> str:
    """Replace entity names in text with their UTF-8 symbols.

    Each entity name is always followed by a slash in the input,
    so a simple string replacement of "Name/" with "symbol/" is
    sufficient and unambiguous.

    Args:
        text: Input text containing entity names.
        mapping: Dictionary of entity names to symbols.

    Returns:
        Text with entity names replaced by symbols.
    """
    result: str = text
    for name, symbol in mapping.items():
        result = result.replace(f"{name}/", f"{symbol}/")
    return result


def check_output_file(output_path: Path, force: bool, quiet: bool) -> bool:
    """Check if output file exists and handle accordingly.

    Args:
        output_path: Path to the output file.
        force: Whether to force overwrite.
        quiet: Whether to suppress prompts.

    Returns:
        True if should proceed, False if should cancel.
    """
    if not output_path.exists():
        return True

    if force:
        return True

    if quiet:
        console.print(
            f"[yellow]Warning:[/yellow] Output file exists: {output_path}. "
            f"Use -f to overwrite."
        )
        return False

    from rich.prompt import Confirm
    return Confirm.ask(
        f"File [cyan]{output_path}[/cyan] exists. Overwrite?"
    )


def process_file(config: Config) -> ProcessingResult:
    """Process a file and replace entity names with symbols.

    Args:
        config: Processing configuration.

    Returns:
        ProcessingResult with status and statistics.
    """
    if not config.quiet:
        console.print(f"[blue]Reading:[/blue] {config.input_path}")

    # Check input file exists
    if not config.input_path.exists():
        return ProcessingResult(
            success=False,
            exit_code=1,
            error_message=f"Input file not found: {config.input_path}",
        )

    # Check input is a file
    if not config.input_path.is_file():
        return ProcessingResult(
            success=False,
            exit_code=1,
            error_message=f"Input path is not a file: {config.input_path}",
        )

    # Check read permissions
    try:
        with open(config.input_path, "rb") as _:
            pass
    except PermissionError:
        return ProcessingResult(
            success=False,
            exit_code=1,
            error_message=f"Permission denied reading: {config.input_path}",
        )
    except OSError as e:
        return ProcessingResult(
            success=False,
            exit_code=1,
            error_message=f"Cannot access input file: {e}",
        )

    # Check output file
    if not check_output_file(config.output_path, config.force, config.quiet):
        return ProcessingResult(
            success=False,
            exit_code=4,
            error_message="User cancelled (file exists)",
        )

    # Check output directory exists and is writable
    output_dir: Path = config.output_path.parent
    if not output_dir.exists():
        return ProcessingResult(
            success=False,
            exit_code=1,
            error_message=f"Output directory does not exist: {output_dir}",
        )

    try:
        # Test write permissions by creating a temp file
        test_file: Path = output_dir / ".write_test"
        test_file.touch()
        test_file.unlink()
    except PermissionError:
        return ProcessingResult(
            success=False,
            exit_code=1,
            error_message=f"Permission denied writing to: {output_dir}",
        )
    except OSError as e:
        return ProcessingResult(
            success=False,
            exit_code=1,
            error_message=f"Cannot write to output directory: {e}",
        )

    # Read file with encoding handling
    try:
        content: str = config.input_path.read_text(encoding="utf-8")
    except UnicodeDecodeError as e:
        return ProcessingResult(
            success=False,
            exit_code=3,
            error_message=f"Encoding error reading {config.input_path}: {e}",
        )
    except PermissionError:
        return ProcessingResult(
            success=False,
            exit_code=1,
            error_message=f"Permission denied reading: {config.input_path}",
        )
    except OSError as e:
        return ProcessingResult(
            success=False,
            exit_code=3,
            error_message=f"Error reading file: {e}",
        )

    lines: list[str] = content.splitlines()
    processed_lines: list[str] = []

    # Process lines
    try:
        if config.show_progress and not config.quiet:
            for line in tqdm(lines, desc="Processing", unit="lines"):
                processed_lines.append(
                    replace_entities_with_symbols(line, config.mapping)
                )
        else:
            processed_lines = [
                replace_entities_with_symbols(line, config.mapping)
                for line in lines
            ]
    except MemoryError:
        return ProcessingResult(
            success=False,
            exit_code=3,
            error_message="Out of memory while processing",
        )
    except Exception as e:
        return ProcessingResult(
            success=False,
            exit_code=3,
            error_message=f"Processing error: {e}",
        )

    processed_content: str = "\n".join(processed_lines) + "\n"

    # Count symbols inserted
    symbols_inserted: int = sum(
        processed_content.count(symbol)
        for symbol in config.mapping.values()
    )

    # Write output with error handling
    try:
        config.output_path.write_text(processed_content, encoding="utf-8")
    except PermissionError:
        return ProcessingResult(
            success=False,
            exit_code=1,
            error_message=f"Permission denied writing: {config.output_path}",
        )
    except FileNotFoundError:
        return ProcessingResult(
            success=False,
            exit_code=3,
            error_message=f"Cannot create output file: {config.output_path}",
        )
    except OSError as e:
        # Handle disk full, I/O errors, etc.
        error_str: str = str(e).lower()
        if "no space left" in error_str or "disk full" in error_str:
            return ProcessingResult(
                success=False,
                exit_code=3,
                error_message=f"Disk full: cannot write {config.output_path}",
            )
        return ProcessingResult(
            success=False,
            exit_code=3,
            error_message=f"Error writing file: {e}",
        )

    if not config.quiet:
        console.print(f"[green]Written:[/green] {config.output_path}")
        console.print(
            f"[dim]Processed {len(lines)} lines, "
            f"{symbols_inserted} symbols inserted[/dim]"
        )

    return ProcessingResult(
        success=True,
        lines_processed=len(lines),
        symbols_inserted=symbols_inserted,
        exit_code=0,
    )


def main(argv: Sequence[str] | None = None) -> int:
    """Main entry point for command-line usage.

    Args:
        argv: Command line arguments. Defaults to sys.argv[1:].

    Returns:
        Exit code (0 for success, non-zero for errors).
    """
    parser: CustomArgumentParser = create_parser()
    args: argparse.Namespace = parser.parse_args(argv)

    if args.long_help:
        parser.print_long_help()
        return 0

    if args.short_help:
        parser.print_help()
        return 0

    # Configure logging
    log_level: int = logging.WARNING
    if args.verbose:
        log_level = logging.DEBUG
    elif args.quiet:
        log_level = logging.ERROR

    logging.basicConfig(
        level=log_level,
        format="%(asctime)s - %(name)s - %(levelname)s - %(message)s",
    )

    # Parse custom mapping if provided
    mapping: dict[str, str] = {}
    if args.mapping:
        try:
            mapping = parse_mapping(args.mapping, args.separator)
        except ValueError as e:
            console.print(f"[red]Error:[/red] {e}")
            return 2

    # Create configuration
    try:
        config: Config = Config(
            input_path=Path(args.input),
            output_path=Path(args.output),
            mapping=mapping,
            force=args.force,
            show_progress=args.progress,
            quiet=args.quiet,
            verbose=args.verbose,
        )
    except (ValueError, TypeError) as e:
        console.print(f"[red]Configuration error:[/red] {e}")
        return 2

    # Process file
    result: ProcessingResult = process_file(config)

    if not result.success:
        console.print(f"[red]Error:[/red] {result.error_message}")
        return result.exit_code

    return 0


if __name__ == "__main__":
    sys.exit(main())

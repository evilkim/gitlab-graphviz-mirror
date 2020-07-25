Push-Location shapes
python shapes.py
Pop-Location

# Larger graph testing
# Python unittest prints to stderr, but Powershell interprets stderr
# messages as an error, failing the pipeline. So we must redirect
# stderr to stdout. The line noise at the end does so. See
# https://stackoverflow.com/a/20950421/171898 for explanation.
python -m pytest large 2>&1 | %{ "$_" }

#!/usr/bin/env bash
set -e

git config core.hooksPath .git_template/hooks
echo "Git hooks configured for this repository."

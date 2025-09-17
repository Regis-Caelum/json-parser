#!/bin/bash
set -e

# Change this to your email
TO_EMAIL="inshal.khan@outlook.com"

# Optional: set your From address if not configured globally
FROM_EMAIL="ikhan@pinnove.com"

# Temporary directory for patch
PATCH_DIR=$(mktemp -d)

# Generate patch of today's commits
git format-patch --since="$(date +%Y-%m-%d) 00:00" --until=now origin/main..HEAD -o "$PATCH_DIR"

# Send patch (all files in PATCH_DIR)
if ls "$PATCH_DIR"/*.patch 1> /dev/null 2>&1; then
    echo "Found patches:"
    ls "$PATCH_DIR"/*.patch

    git send-email \
        --from="$FROM_EMAIL" \
        --to="$TO_EMAIL" \
        "$PATCH_DIR"/*.patch
else
    echo "No commits in the specified range → No patches to send."
fi

rm -rf "$PATCH_DIR"

# Clean up
rm -rf "$PATCH_DIR"

#!/bin/bash
# test_auth.sh
SERVER="./ircserv 4444 password"

# Start the server in the background
$SERVER &
SERVER_PID=$!

# Allow server to start
sleep 2

# Test authentication
echo "Testing authentication..."
nc 127.0.0.1 4444 << EOF
NICK testuser
USER testuser 0 * :Real Name
PASS wrongpassword
EOF

# Check for expected failure response
EXPECTED=":irc.example.com 464 * :Password incorrect"
RESPONSE=$(nc -w 2 127.0.0.1 4444)

if [[ "$RESPONSE" == *"$EXPECTED"* ]]; then
    echo -e "\033[32mAuthentication test passed\033[0m"
else
    echo -e "\033[31mAuthentication test failed\033[0m"
fi

# Kill the server
kill $SERVER_PID

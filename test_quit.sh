#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

SERVER_PORT=8080
SERVER_PASSWORD=password

# サーバーを確実に終了する関数
stop_server() {
    if [ ! -z "$SERVER_PID" ]; then
        kill -TERM $SERVER_PID 2>/dev/null
        sleep 2
        kill -KILL $SERVER_PID 2>/dev/null
    fi
}

# クリーンアップ関数
cleanup() {
    stop_server
    echo "Cleanup completed."
    exit
}

# Ctrl+C などでスクリプトが中断された場合のクリーンアップ
trap cleanup SIGINT SIGTERM

# サーバーの起動
./ircserv $SERVER_PORT $SERVER_PASSWORD &
SERVER_PID=$!

# サーバーの起動を待つ
for i in {1..10}; do
    if nc -z localhost $SERVER_PORT; then
        echo "Server started successfully."
        break
    fi
    if [ $i -eq 10 ]; then
        echo "Failed to start server."
        cleanup
    fi
    sleep 1
done

# テスト関数
test_quit() {
    local nickname=$1
    local expected_output=$2

    output=$(echo -e "NICK $nickname\r\nUSER $nickname 0 * :Real Name\r\nQUIT :Goodbye\r\n" | nc localhost $SERVER_PORT)

    if echo "$output" | grep -q "$expected_output"; then
        echo -e "${GREEN}PASS: QUIT test for $nickname${NC}"
    else
        echo -e "${RED}FAIL: QUIT test for $nickname${NC}"
        echo "Expected: $expected_output"
        echo "Got: $output"
    fi
}

# テストケース
test_quit "user1" "ERROR :Closing Link"
test_quit "user2" "ERROR :Closing Link"

# 複数のクライアントを接続してからQUITを実行
{
    echo -e "NICK user3\r\nUSER user3 0 * :Real Name\r\n"
    sleep 1
    echo -e "NICK user4\r\nUSER user4 0 * :Real Name\r\n"
    sleep 1
    echo -e "QUIT :Goodbye\r\n"
} | nc localhost $SERVER_PORT &

sleep 2

# user4がuser3のQUITメッセージを受信したか確認
output=$(echo -e "NICK user4\r\nUSER user4 0 * :Real Name\r\nJOIN #test\r\nPRIVMSG #test :Are you there?\r\n" | nc localhost $SERVER_PORT)
if echo "$output" | grep -q "user3.*QUIT :Goodbye"; then
    echo -e "${GREEN}PASS: QUIT message broadcast test${NC}"
else
    echo -e "${RED}FAIL: QUIT message broadcast test${NC}"
fi

# クリーンアップと終了
cleanup

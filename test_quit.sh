#!/bin/bash

# IRCサーバーの起動
./ircserv 8080 password &
SERVER_PID=$!

# サーバーの起動を待つ
sleep 2

# テスト関数
test_quit() {
    local nickname=$1
    local expected_output=$2

    # クライアントの接続とQUITコマンドの実行
    output=$(echo -e "NICK $nickname\r\nUSER $nickname 0 * :Real Name\r\nQUIT :Goodbye\r\n" | nc localhost 8080)

    # 期待される出力と実際の出力を比較
    if echo "$output" | grep -q "$expected_output"; then
        echo "PASS: QUIT test for $nickname"
    else
        echo "FAIL: QUIT test for $nickname"
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
} | nc localhost 8080 &

sleep 2

# user4がuser3のQUITメッセージを受信したか確認
output=$(echo -e "PRIVMSG user4 :Are you there?\r\n" | nc localhost 6667)
if echo "$output" | grep -q "user3.*QUIT :Goodbye"; then
    echo "PASS: QUIT message broadcast test"
else
    echo "FAIL: QUIT message broadcast test"
fi

# サーバーの停止
kill $SERVER_PID

echo "All tests completed."

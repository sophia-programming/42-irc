# 42-irc

## The Project
Internet Relay Chat or IRC is a text-based communication protocol on the Internet.

## How to get started
Do make and then run:

`./ircserv <port> <password>`


## Command Usage

### NICK
ユーザーのニックネームを設定または変更する
`NICK <nickname>`


### USER
サーバーにログインするために使用するユーザー情報を設定する
`USER <username> <hostname> <servername> :<real name>`


### PASS


### JOIN
特定のチャンネルに参加もしくは作成する
`JOIN #<channelname> [key]`


### PRIVMSG
特定のユーザーやチャンネルにメッセージを送信する
`PRIVMSG <recipient> :<message>`

### QUIT
サーバーから切断する
`QUIT :[optional message]`

### KICK
特定のユーザーをチャンネルから追放する
オペレーター権限が必要
`KICK #<channelname> <username> :[optional reason]`

### TOPIC
チャンネルのトピックを設定または表示する
```
TOPIC #<channelname> :<new topic>
TOPIC #<channelname>
```


### MODE
チャンネルに対してモード（権限や設定）を変更する
```
MODE #<channelname> +/-<mode> <parameters>
MODE <username> +<mode>
```
 - i :招待のみチャンネルに設定/削除する
 - t :TOPICコマンドの使用を制限/解除する
 - k :チャンネルkeyを設定/削除する
 - o :チャンネルオペレーター権限を付与/削除する
 - l :チャンネルの人数制限を設定/削除する

### INVITE
特定のユーザーをプライベートチャンネルに招待する
`INVITE <username> #<channelname>`

### PONG
サーバーからの PING コマンドへの応答
クライアントとサーバーの接続が維持されていることを確認するための機能
`PONG :<server>`


### NOTICE
ユーザーやチャンネルにメッセージを送信する
PRIVMSGとの違いは応答が期待されないシステム通知や自動メッセージの送信に使用される
`NOTICE <target> :<message>`
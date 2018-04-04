#!./test/libs/bats/bin/bats

load 'libs/bats-support/load'

@test "Version" {
  run bash -c 'echo ">version" | nc -v 127.0.0.1 4711'
  echo "output: ${lines[@]}"
  [[ ${lines[0]} == "Connection to 127.0.0.1 4711 port [tcp/*] succeeded!" ]]
  [[ ${lines[1]} =~ "version" ]]
  [[ ${lines[2]} =~ "tag" ]]
  [[ ${lines[3]} =~ "branch" ]]
  [[ ${lines[4]} =~ "date" ]]
  [[ ${lines[5]} == "---EOM---" ]]
}

@test "DB test: Tables created and populated?" {
  run bash -c 'sqlite3 pihole-FTL.db .dump'
  echo "output: ${lines[@]}"
  [[ "${lines[@]}" == *"CREATE TABLE queries ( id INTEGER PRIMARY KEY AUTOINCREMENT, timestamp INTEGER NOT NULL, type INTEGER NOT NULL, status INTEGER NOT NULL, domain TEXT NOT NULL, client TEXT NOT NULL, forward TEXT );"* ]]
  [[ "${lines[@]}" == *"CREATE TABLE ftl ( id INTEGER PRIMARY KEY NOT NULL, value BLOB NOT NULL );"* ]]
  [[ "${lines[@]}" == *"CREATE TABLE counters ( id INTEGER PRIMARY KEY NOT NULL, value INTEGER NOT NULL );"* ]]
  [[ "${lines[@]}" == *"INSERT INTO \"counters\" VALUES(0,0);"* ]]
  [[ "${lines[@]}" == *"INSERT INTO \"counters\" VALUES(1,0);"* ]]
  [[ "${lines[@]}" == *"INSERT INTO \"ftl\" VALUES(0,2);"* ]]
  [[ "${lines[@]}" == *"CREATE INDEX idx_queries_timestamps ON queries (timestamp);"* ]]
}

@test "Arguments check: Invalid option" {
  run bash -c './pihole-FTL abc'
  echo "output: ${lines[@]}"
  [[ ${lines[0]} == "pihole-FTL: invalid option -- 'abc'" ]]
  [[ ${lines[1]} == "Try './pihole-FTL --help' for more information" ]]
}

@test "Help argument return help text" {
  run bash -c './pihole-FTL help'
  echo "output: ${lines[@]}"
  [[ ${lines[0]} == "pihole-FTL - The Pi-hole FTL engine" ]]
}

@test "Unix socket returning data" {
  run bash -c './socket-test travis'
  echo "output: ${lines[@]}"
  [[ ${lines[0]} == "Socket created" ]]
  [[ ${lines[1]} == "Connection established" ]]
  [[ ${lines[2]} == "d2 ff ff ff ff d2 00 00 00 07 d2 00 00 00 02 ca 41 e4 92 49 d2 00 00 00 06 d2 00 00 00 03 d2 00 00 00 02 d2 00 00 00 03 d2 00 00 00 03 cc 02 c1 " ]]
}

@test "Verify no FATAL warnings are present in the generated log" {
  run bash -c 'grep -c "FATAL" pihole-FTL.log'
  echo "output: ${lines[@]}"
  [[ ${lines[0]} == "0" ]]
}

@test "Final part of the tests: Killing pihole-FTL process" {
  run bash -c 'echo ">kill" | nc -v 127.0.0.1 4711'
  echo "output: ${lines[@]}"
  [[ ${lines[0]} == "Connection to 127.0.0.1 4711 port [tcp/*] succeeded!" ]]
  [[ ${lines[1]} == "killed" ]]
  [[ ${lines[2]} == "---EOM---" ]]
}

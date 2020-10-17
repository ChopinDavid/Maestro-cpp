require('dotenv').config()
const https = require('https');
const personalToken = process.env.PERSONAL_TOKEN;
const baseUrl = 'lichess.org';
var StringDecoder = require('string_decoder').StringDecoder;
var decoder = new StringDecoder('utf8');
const engineExePath = 'C:\\Users\\chopi\\Desktop\\chess-engine\\maestro\\uci.exe';
const childProcess = require('child_process');
const spawn_options = {
    cwd: null, env: null, detached: false
}
var engineIsReady = false;
var inGame = false;
var currentOpponentUsername;
var isWhite;
var moveCount;
var currentGameId;

var engineStream;

function startEngine(callback) {
    engineStream = childProcess.spawn(engineExePath, [], spawn_options);
    engineStream.stdout.on('data', function (data) {
        var result = decoder.write(data);
        console.log(`RESULT: ${result}`);
        if (result == 'id name Maestro\r\nid author dvdutch\r\nuciok\r\n') {
            engineStream.stdin.write('isready\n');
        } else if (result == 'readyok\r\n') {
            engineIsReady = true;
            callback();
        } else if (result.substring(0, 8) === "bestmove") {
            let move = result.split(" ")[1].trim();
            makeMove(move, currentGameId);
        } else {
            console.log('no match');
        }
    });

    engineStream.stdin.write('uci\n');
}

function endEngine() {
    console.log("ending engine");
    engineStream = null;
    engineIsReady = false;
}

function acceptChallenge(id) {
    var options = {
        host: baseUrl,
        path: `/api/challenge/${id}/accept`,
        headers: { 'Authorization': 'Bearer ' + personalToken },
        method: 'POST'
    };
    var request = https.request(options);

    request.on('error', function (err) {
        // Handle error
        console.log(err);
    });

    request.end();
}

function createChallenge(username) {
    var options = {
        host: baseUrl,
        path: `/api/challenge/${username}?rated=true&clock.limit=300&clock.increment=0`,
        headers: { 'Authorization': 'Bearer ' + personalToken },
        method: 'POST'
    };
    var request = https.request(options);

    request.on('error', function (err) {
        // Handle error
        console.log(err);
    });

    request.end();
}

function streamGame(id) {
    var options = {
        host: baseUrl,
        path: `/api/bot/game/stream/${id}`,
        headers: { 'Authorization': 'Bearer ' + personalToken }
    };
    https.get(options, function (res) {
        res.on('error', function (err) {
            console.log(err);
        });
        res.on('data', function (chunk) {
            var textChunk = chunk.toString('utf8');
            try {
                var jsonChunk = JSON.parse(textChunk);
                console.log("game stream: ");
                console.log(jsonChunk);
                switch (jsonChunk.type) {
                    case 'gameState':
                        if (jsonChunk.status == "started") {
                            let moves = (jsonChunk.moves).split(" ");
                            if (moves.length == 0) {
                                break;
                            }
                            let whiteToMove = moves.length % 2 == 0 ? true : false;

                            if ((whiteToMove && isWhite) || (!whiteToMove && !isWhite)) {
                                console.log("writing to engine " + jsonChunk.moves);
                                engineStream.stdin.write(`position startpos moves ${jsonChunk.moves}\n`);
                                engineStream.stdin.write(`go wtime ${jsonChunk.wtime} btime ${jsonChunk.btime} winc ${jsonChunk.winc} binc ${jsonChunk.binc}\n`);
                            }
                            break;
                        }

                    case 'gameFull':
                        inGame = true;
                        moveCount = 0;
                        currentGameId = jsonChunk.id;
                        if (jsonChunk.white.id == "maestrobot") {
                            console.log("iswhite");
                            isWhite = true;
                            currentOpponentUsername = jsonChunk.black.name;
                            engineStream.stdin.write('position startpos\n');
                            engineStream.stdin.write(`go wtime ${jsonChunk.state.wtime} btime ${jsonChunk.state.btime} winc ${jsonChunk.state.winc} binc ${jsonChunk.state.binc}\n`);
                        } else {
                            currentOpponentUsername = jsonChunk.white.name;
                            console.log("isblack");
                            isWhite = false;
                            if (jsonChunk.state.moves != null) {
                                engineStream.stdin.write(`position startpos moves ${jsonChunk.state.moves}\n`);
                                engineStream.stdin.write(`go wtime ${jsonChunk.state.wtime} btime ${jsonChunk.state.btime} winc ${jsonChunk.state.winc} binc ${jsonChunk.state.binc}\n`);
                            }
                        }
                    default:
                        break;
                }
            } catch (e) {
                if (e != "SyntaxError: Unexpected end of JSON input") {
                    console.log(`unable to convert to JSON: ${e}`);
                }
            } finally {

            }
        });
        res.on('end', function () {
            // all data has been downloaded
        });
    });
}

function makeMove(moveString, gameId) {
    var options = {
        host: baseUrl,
        path: `/api/bot/game/${gameId}/move/${moveString}`,
        headers: { 'Authorization': 'Bearer ' + personalToken },
        method: 'POST'
    };
    var request = https.request(options);

    request.on('data', function (chunk) {
        var textChunk = chunk.toString('utf8');
    });

    request.on('error', function (err) {
        // Handle error
        console.log(err);
    });

    request.end();
}

function go() {
    var options = {
        host: baseUrl,
        path: '/api/stream/event',
        //This is the only line that is new. `headers` is an object with the headers to request
        headers: { 'Authorization': 'Bearer ' + personalToken },
        method: 'GET'
    };

    https.get(options, function (res) {
        res.on('error', function (err) {
            console.log(err);
        });
        res.on('data', function (chunk) {
            var textChunk = chunk.toString('utf8');
            try {
                var jsonChunk = JSON.parse(textChunk);
                console.log("state stream: ");
                console.log(jsonChunk);
                switch (jsonChunk.type) {
                    case 'challenge':
                        if (!inGame) {
                            startEngine(() => {
                                acceptChallenge(jsonChunk.challenge.id);
                            });
                        }
                        break;
                    case 'gameStart':
                        streamGame(jsonChunk.game.id);
                        break;
                    case 'gameFinish':
                        var tempOpponentUsername = currentOpponentUsername;
                        currentOpponentUsername = null;
                        inGame = false;
                        endEngine();
                        startEngine(() => {
                            createChallenge(tempOpponentUsername);
                        });

                    case 'challengeDeclined':
                        endEngine();
                    default:
                        break;
                }
            } catch (e) {
                if (e != "SyntaxError: Unexpected end of JSON input") {
                    console.log(`unable to convert to JSON: ${e}`);
                }
            } finally {

            }
        });
        res.on('end', function () {
            // all data has been downloaded
        });
    });
}

go();
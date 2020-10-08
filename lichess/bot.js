const https = require('https');
const personalToken = 'WmE0udZGdBY8QAEI';
const baseUrl = 'lichess.org';
const engineExePath = 'C:\\Users\\chopi\\Desktop\\chess-engine\\maestro\\uci.exe';
const childProcess = require('child_process');
const spawn_options = {
    cwd: null, env: null, detached: false
}
var engineIsReady = false;
var inGame = false;
var isWhite;
var moveCount;
var currentGameId;

const engineStream = childProcess.spawn(engineExePath, [], spawn_options);
engineStream.stdout.on('data', function (data) {
    var result = data.toString();
    console.log(`RESULT: ${result}`);
    if (result == 'id name Maestro\r\nid author dvdutch\r\nuciok\r\n') {
        engineStream.stdin.write('isready\n');
    } else if (result == 'readyok\r\n') {
        engineIsReady = true;
    } else if (result.substring(0, 8) === "bestmove") {
        let move = result.split(" ")[1].trim();
        makeMove(move, currentGameId);
    } else {
        console.log('no match');
    }
});

engineStream.stderr.on('data', function (data) {
    var result = data.toString();
    console.log(`ERROR: ${result}`);
});

engineStream.stdin.write('uci\n');

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
                        let moves = (jsonChunk.moves).split(" ");
                        let whiteToMove = moves.length % 2 == 0 ? true : false;
                        if ((whiteToMove && isWhite) || (!whiteToMove && !isWhite)) {
                            console.log("writing to engine " + jsonChunk.moves);
                            engineStream.stdin.write(`position startpos moves ${jsonChunk.moves}\n`);
                        }
                        break;
                    case 'gameFull':
                        inGame = true;
                        moveCount = 0;
                        currentGameId = jsonChunk.id;
                        if (jsonChunk.white.id == "maestrobot") {
                            console.log("iswhite");
                            isWhite = true;
                            engineStream.stdin.write('position startpos\n');
                        } else {
                            console.log("isblack");
                            isWhite = false;
                        }
                        engineStream.stdin.write('ucinewgame\nisready\n');
                    default:
                        break;
                }
            } catch (e) {
                console.log(`unable to convert to JSON: ${e}`);
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
                        if (engineIsReady && !inGame) {
                            acceptChallenge(jsonChunk.challenge.id);
                        }
                        break;
                    case 'gameStart':
                        streamGame(jsonChunk.game.id);
                        break;
                    case 'gameFinish':
                        inGame = false;
                    default:
                        break;
                }
            } catch (e) {
                console.log(`unable to convert to JSON: ${e}`);
            } finally {

            }
        });
        res.on('end', function () {
            // all data has been downloaded
        });
    });
}

go();
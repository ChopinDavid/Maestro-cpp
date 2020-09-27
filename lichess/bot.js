const https = require('https');
const personalToken = 'WmE0udZGdBY8QAEI';
const baseUrl = 'lichess.org';


function acceptChallenge(id) {
    var options = {
        host: baseUrl,
        path: `/api/challenge/${id}/accept`,
        headers: { 'Authorization': 'Bearer ' + personalToken },
        method: 'POST'
    };
    var request = https.request(options, function (res) {
        res.setEncoding('utf8');
        res.on('data', function (chunk) {
            console.log('Response: ' + chunk);
        });
    }
    );

    request.on('error', function(err) {
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
                console.log(jsonChunk);
                if (jsonChunk.type == 'challenge') {
                    acceptChallenge(jsonChunk.challenge.id);
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
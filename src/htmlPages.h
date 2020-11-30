// In <script> insert  line 164 const ip = { your ip}

char index_page[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="pt-br">

<head>
    <title>Lab-Plugs</title>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <style>
        /* RESER PRESET */

        * {
            padding: 0;
            margin: 0;
            box-sizing: border-box;
            font-size: 100%;
            font-family: Arial, Helvetica, sans-serif;
        }

        ul {
            list-style: none;
        }

        a {
            text-decoration: none;
        }

        body {
            background-color: #f2f2f2;
        }

        /** HEADER **/

        header {
            display: flex;
            height: 60px;
            align-items: center;
            justify-content: center;
            background: linear-gradient(135deg, #a9a9a9, #d9d9d9);
        }

        header h1 {
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 22px;
            gap: 10px;

        }

        header svg {
            height: 50px;
            margin-top: 5px;
        }

        /** SECTION **/

        section {
            display: flex;
            margin: 60px auto;
            align-items: center;
            justify-content: center;
            background: linear-gradient(135deg, #a9a9a9, #d9d9d9);
            max-width: 453.04px;
            border-radius: 5px;
            box-shadow: 10px 10px 5px #88888877;

        }

        section ul {
            display: flex;
            flex-direction: column;
            margin: 40px auto;
            width: 70%;
        }

        section li {
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            width: 100%;
            display: flex;
            background: #404040;
            color: #d9d9d9;
            padding: 8px;
            border-radius: 5px;
            cursor: pointer;
        }

        li+li {
            margin-top: 25px;
        }

        /* FOOTER */

        footer {
            text-align: center;
            width: 100%;
        }

        footer a {
            color: #5d70ac;
        }
    </style>
</head>

<body>
    <header>
        <h1>
            <a href="#">
                <?xml version="1.0" encoding="iso-8859-1"?>
                <!-- Generator: Adobe Illustrator 19.0.0, SVG Export Plug-In . SVG Version: 6.00 Build 0)  -->
                <svg version="1.1" id="Capa_1" xmlns="http://www.w3.org/2000/svg"
                    xmlns:xlink="http://www.w3.org/1999/xlink" x="0px" y="0px" viewBox="0 0 512 512"
                    style="enable-background:new 0 0 512 512;" xml:space="preserve">
                    <g>
                        <g>
                            <path d="M289.328,290.839c-5.857-5.857-15.355-5.857-21.213,0l-34.836,34.836l-46.953-46.953l34.836-34.836
			c5.858-5.857,5.858-15.355,0-21.213c-5.857-5.857-15.355-5.857-21.213,0l-34.836,34.836l-12.115-12.115
			c-0.008-0.008-11.361-11.359-11.361-11.359c-5.857-5.857-15.355-5.857-21.213,0c-5.857,5.858-5.857,15.355,0,21.214l0.753,0.753
			l-46.196,46.197c-33.563,33.563-36.81,86.123-9.764,123.375l-60.823,60.82c-5.858,5.858-5.858,15.355,0,21.214
			c2.93,2.929,6.768,4.393,10.607,4.393s7.678-1.465,10.606-4.393l60.811-60.81c16.172,11.78,35.583,18.145,55.975,18.145
			c0.002,0,0,0,0.001,0c25.462,0,49.403-9.917,67.407-27.921l46.202-46.199l0.753,0.754c2.93,2.929,6.768,4.392,10.607,4.392
			c3.838,0,7.678-1.464,10.606-4.392c5.857-5.858,5.857-15.355-0.001-21.214l-23.476-23.476l34.835-34.836
			C295.186,306.194,295.186,296.697,289.328,290.839z" />
                        </g>
                    </g>
                    <g>
                        <g>
                            <path d="M507.608,4.394c-5.857-5.858-15.355-5.858-21.214,0l-60.811,60.808c-16.173-11.779-35.584-18.144-55.977-18.144
			c-25.463,0-49.404,9.918-67.408,27.922l-46.197,46.199l-0.755-0.755c-5.857-5.857-15.355-5.857-21.213,0.001
			c-5.857,5.857-5.857,15.355,0,21.213l136.33,136.329c2.93,2.929,6.768,4.393,10.607,4.393c3.838,0,7.678-1.464,10.605-4.393
			c5.858-5.858,5.858-15.355,0-21.213l-0.754-0.755l46.197-46.195c33.563-33.561,36.81-86.121,9.766-123.375l60.822-60.821
			C513.465,19.75,513.465,10.253,507.608,4.394z" />
                        </g>
                    </g>
                </svg>

            </a>
            Plugs
        </h1>
    </header>

    <section>
        <ul>
            <li id="p1" onclick="toogle(1)">Soldering Iron</li>
            <li id="p2" onclick="toogle(2)" href="#">Hot Glue</li>
            <li id="p3" onclick="toogle(3)" href="#">Table Lights</li>
            <li id="p4" onclick="toogle(4)" href="#">Power Plug</li>
        </ul>
    </section>

    <footer>
        <small>&copy By tpaphysics - <a href="https://github.com/tpaphysics/iot_esp8266_plugs">GitHub</a></small>

    </footer>

    <script>
        const ip = "192.168.0.50";


        async function verifyButtonStates() {
            const url = `http://${ip}/states`;

            try {
                const res = await fetch(url);
                const data = await res.json();

                for (let i = 0; i < 4; i++) {
                    const btn = document.getElementById(`p${1 + i}`);

                    if (data.statePlugs[i] == 0) {
                        btn.style.backgroundColor = '#5d70ac';
                    } else {
                        btn.style.backgroundColor = '#404040';
                    };

                };
            } catch (err) {

                for (let i = 0; i < 4; i++) {
                    const btn = document.getElementById(`p${1 + i}`);
                    btn.style.backgroundColor = '#1a1a1a';
                };

            };
        };

        function setStates(number, states) {
            const btn = document.getElementById(`p${number}`);

            let index = number - 1;
            if (states[index] == 0) {
                btn.style.backgroundColor = '#5d70ac';
            } else {
                btn.style.backgroundColor = '#404040';
            };
        };

        async function toogle(number) {
            const url = `http://${ip}/toogle?plug=${number}`;

            try {

                const res = await fetch(url);
                const data = await res.json();

                const states = data.statePlugs;
                setStates(number, states);
                console.log(states);

            } catch (err) {

                for (let i = 0; i < 4; i++) {
                    const btn = document.getElementById(`p${1 + i}`);
                    btn.style.backgroundColor = '#1a1a1a';
                };
            };
        };


        verifyButtonStates();
        var interval = window.setInterval(verifyButtonStates, 10000);
    
    </script>
</body>

</html>

)=====";

char update_page[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="pt-br">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Firmware Update</title>
    <style>
        * {
            padding: 0;
            margin: 0;
            font-size: 100%;
            box-sizing: border-box;
        }

        header {
            display: flex;
            height: 60px;
            align-items: center;
            justify-content: center;
            background: linear-gradient(135deg, #a9a9a9, #d9d9d9);
        }

        body {
            background-color: #f2f2f2;
        }

        header h1 {
            font-size: 22px;
        }

        form {
            display: flex;
            flex-direction: column;
            margin: 60px auto;
            padding: 100px;
            align-items: center;
            justify-content: space-between;
            background: linear-gradient(135deg, #a9a9a9, #d9d9d9);
            max-width: 453.04px;
            height: 290px;
            border-radius: 5px;
            box-shadow: 10px 10px 5px #88888877;
        }

        input {
            padding: 2px;


        }
    </style>
</head>

<body>
    <header>
        <h1>Update Firmware</h1>
    </header>

    <form method='POST' action='/update' enctype='multipart/form-data'>
        <input type='file' name='update'>
        <input type='submit' value='Update'>
    </form>
</body>

</html>    

)=====";
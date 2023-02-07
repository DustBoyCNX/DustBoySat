const BinParser = require("binary-parser").Parser;
import { DelimiterParser, SerialPort } from "serialport";

const toHexString = function (arr: any) {
  return Buffer.from(arr).toString("hex");
};

const port = new SerialPort({
  path: "/dev/ttyUSB0",
  baudRate: 9600,
  parity: "none",
  stopBits: 1,
  dataBits: 8,
  autoOpen: false,
});

let output: string;
let mode: string;

const binaryPacket = port.pipe(
  new DelimiterParser({ delimiter: Buffer.from("66", "hex") })
);

const asciiPacket = port.pipe(
  new DelimiterParser({ delimiter: Buffer.from("0a", "hex") })
);

port.on("open", () => {
  console.log("Port open");
});

port.on("data", (data) => {
  console.log("RAW", data.toString());
});

asciiPacket.on("data", (data) => {
  if (output === "ASCII") {
    const packet = Buffer.concat([data, Buffer.from("0a", "hex")]);
    console.log(data.toString(), packet);
  }
});

function delay(ms: number) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

output = "ASCII";
mode = "MOCKUP";

async function run() {
  await port.open();
  await delay(100);

  // command packet 4bytes = `s!!
  const packet = ['`'.charCodeAt(0), 's'.charCodeAt(0), 255, '!'.charCodeAt(0)];

  const sleepCmd = Buffer.from(packet);

  port.write(sleepCmd);

  await delay(100);
}

run();

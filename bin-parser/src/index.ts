const BinParser = require("binary-parser").Parser;
import { DelimiterParser, SerialPort } from "serialport";

const toHexString = function (arr: any) {
  return Buffer.from(arr).toString("hex");
};

const BINARY = new BinParser()
  .endianess("little")
  .array("header", {
    type: "uint8",
    length: 1,
    formatter: toHexString,
  })
  .uint8("counter")
  .uint16("pm10")
  .uint16("pm2_5")
  .uint16("pm1")
  .array("tail", {
    type: "uint8",
    length: 1,
    formatter: toHexString,
  });
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
  //   console.log("RAW", data);
});

asciiPacket.on("data", (data) => {
  if (output === "ASCII") {
    const packet = Buffer.concat([data, Buffer.from("0a", "hex")]);
    console.log(data.toString(), packet);
  }
});

binaryPacket.on("data", (data) => {
  if (data.length != 8) return;
  if (output === "BIN") {
    try {
      const packet = Buffer.concat([data, Buffer.from("66", "hex")]);
      console.log(packet, BINARY.parse(packet));
    } catch (err) {
      console.log("ERR:", err);
    }
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

  if (output === "BIN") {
    port.write("`mB!");
  } else if (output === "ASCII") {
    port.write("`mA!");
  }
  await delay(100);
  if (mode === "MOCKUP") {
    console.log("RUN MODE MOCKUP");
    port.write("`mM!");
  } else {
    console.log("RUN MODE NORMAL");
    port.write("`mN!");
  }
  await delay(100);
}

run();

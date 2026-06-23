const fs = require("fs");
const { generatePairs } = require("./generate_input.js");
const { haversine } = require("./reference_haversine.js");

const dataFilename = "input.json";
const answerFile = "answer";

const count = Number(process.argv[2]);
let range = process.argv[3] === undefined ? 100 : Number(process.argv[3]);
const mode = process.argv[4] === "cluster" ? "cluster" : "globe";

if (!Number.isInteger(count) || count <= 0) {
  console.error("Usage: nodejs main.js [amount of data] [range] [mode of generation]");
  process.exit(1);
}

if (!Number.isInteger(range)) {
  console.error("[range] must be a number");
  process.exit(1);
}

if (range < 0) range *= -1;
if (range > 100) range %= 100;

//generating json file
const pairs = generatePairs(count, range, mode);
fs.writeFileSync(dataFilename, JSON.stringify({ pairs }, null, 2));

//generating answer
let sum = 0;

fs.writeFileSync(answerFile, "");
for (const pair of pairs) {
  sum += haversine(pair.x0, pair.y0, pair.x1, pair.y1);
  fs.appendFileSync(answerFile, `${sum}\n`);
}

const average = sum / pairs.length;
fs.appendFileSync(answerFile, `Average: ${average}\n`);


console.log(`Generated ${count} pairs in input.json`);
console.log(`Mode: ${mode}`);
console.log(`Range: ${range}`);
console.log(`Average distance: ${average}`);

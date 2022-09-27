#!/usr/bin/env node
const fs = require('fs');

// idk if this actually will be ever finished

// https://stackoverflow.com/a/32567789
const IsLetter = c => {
	console.log(c)
	return c.toLowerCase() != c.toUpperCase();
};

const ParseArgv = _ => {
	let Argv = process.argv.slice(2);
	let Args = {};
	for (let i = 0; i < Argv.length; i++) {
		let Arg = Argv[i].split("=");
		let Left = Arg[0];
		let Right = Arg.slice(1).join("=");
		Args[Left] = Right;
	};
	return Args;
};

const FillArgs = Args => {
	if (Args["Input"] == undefined) {
		console.log("No input file specified.");
		process.exit(1);
	};
	if (Args["Output"] == undefined) {
		Args["Output"] = Args["Input"] + ".c";
	};
	return Args;
};

const ParseArgs = _ => {
	let Argv = ParseArgv();
	let Args = {};
	Object.keys(Argv).forEach(function(e) {
		console.log(e)
		if (e == "-i") Args["Input"] = Argv[e];
		else if (e == "-o") Args["Output"] = Argv[e];
		else if (e == "-t") Args["Target"] = Argv[e];
	});
	return FillArgs(Args);
};

const CheckMethod = (Keyword, Line) => {
	if (Line.startsWith(Keyword+" ") || Line.startsWith(Keyword+"\t")) {
		return true;
	} else {
		return false;
	}
};

const Transpile = Source => {
	let Final = "";
	let Lines = Source.split('\n');
	for (let i = 0; i < Lines.length; i++) {
		let l = Lines[i];
		let lt = l.trim();
		console.log(lt);
		if (CheckMethod("if", lt)) {
		}
		else if (CheckMethod("elif", lt)) {
		}
		else if (CheckMethod("else", lt)) {
		}
		else if (CheckMethod("for", lt)) {
		}
		else if (CheckMethod("while", lt)) {
		}
		else if (IsLetter(lt[0])) {
		}
		else {
			console.log("Unknown statement at line "+i);
		}
	}
	return Final;
}

const Main = _ => {
	let Args = ParseArgs();
	console.log(Args);
	let Source = fs.readFileSync(Args["Input"], 'utf8');
	let Final = Transpile(Source);
};

Main();
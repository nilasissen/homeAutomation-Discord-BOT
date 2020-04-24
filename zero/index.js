const Discord = require('discord.js');
const fs = require('fs');
const {token, ip, prefix } = require('./config.json');
const WebSocket = require('ws');
const client = new Discord.Client();
client.commands = new Discord.Collection();
const connection = new WebSocket('ws://'+ip+':81/', ['arduino']);

const commandFiles = fs.readdirSync('./commands').filter(file => file.endsWith('.js'));

/**
 * websocket connecting to esp8266 NodeMCU
 */
connection.on('open', function open() {
  // connection.send('something');
});

connection.on('close', function close() {
  console.log('disconnected');
});

connection.on('error', console.error);

connection.on('connected',console.log);

connection.on('message', function incoming(data) {
  console.log(data);
});


/**
 * connecting to Discord server Zero Bot
 */
for (const file of commandFiles) {
	const command = require(`./commands/${file}`);
	// set a new item in the Collection
	// with the key as the command name and the value as the exported module
	client.commands.set(file.split('.')[0], command);
}
client.on('ready', () => {
  console.log(`Logged in as ${client.user.tag}!`);
});

client.on('message', msg => {
  if (!msg.content.startsWith(prefix) || msg.author.bot) return;
  
  const args = msg.content.slice(prefix.length).split(/ +/);
  
  const command = args.shift().toLowerCase();
  try {
    client.commands.get(command)[args[0]](msg, args,connection);
  } catch(error) {
    console.error(error);
	  message.reply('there was an error trying to execute that command!');
  }
});

client.login(token);
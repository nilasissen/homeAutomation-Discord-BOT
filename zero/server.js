const express = require('express');
const config = require('./config.json');
const app = express();
const port  = config.port;
const bot = require('./index');

app.listen(port,()=>{
    console.log("running server at"+port);
});
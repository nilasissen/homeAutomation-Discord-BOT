module.exports =  {
    light: (message, args, connection) => {
        connection.send('1');
        message.channel.send('Turning off the light');
        
    },
    fan: (message, args, connection) => {
        connection.send('2');
        message.channel.send('Turning off the fan.');
        
    },
    both: (message, args, connection) => {
        connection.send('3');
        message.channel.send('Turning off both of the Fan & Light.');
        
    } 
};
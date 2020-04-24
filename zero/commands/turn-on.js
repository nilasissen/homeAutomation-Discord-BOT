module.exports =  {
    light: (message, args, connection) => {
        connection.send('L');
        message.channel.send('turning the light on dumb ass.');
    },
    fan: (message, args, connection) => {
        connection.send('F');
        message.channel.send('turning the fan on dumb ass.');
    },
    both: (message, args, connection) => {
        connection.send('B');
        message.channel.send('Turning on both Fan & Light.');
        
    }
};
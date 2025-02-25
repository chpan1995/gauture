const express = require("express")
const cors = require("cors")
const userRouter = require("./routes/userRouter")
const labTaskRouter = require("./routes/labTaskRouter")
const grianTyoeRouter = require("./routes/GrianTyoeRouter")
const otherRouter = require('./routes/otherRouter')
const tagContorller = require('./routes/tagsRouter')

const logger = require('./logging/logging')
require('./webscoket')

const app = express();

app.use(cors());
// parse requests of content-type - application/json
app.use(express.json()); /* bodyParser.json() is deprecated */
// parse requests of content-type - application/x-www-form-urlencoded
app.use(express.urlencoded({ extended: true }));

app.use(express.static(__dirname+'/pack'));

otherRouter(app);
userRouter(app);
labTaskRouter(app);
grianTyoeRouter(app);
tagContorller(app);

const PORT = process.env.PORT || 8080;
app.listen(PORT, () => {
    logger.info(`HTTPServer is running on port ${PORT}.`);
});

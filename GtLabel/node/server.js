const express = require("express")
const cors = require("cors")
const userRouter = require("./routes/userRouter")
const labTaskRouter = require("./routes/labTaskRouter")
const app = express();
const logger = require('./logging/logging')

app.use(cors());
// parse requests of content-type - application/json
app.use(express.json()); /* bodyParser.json() is deprecated */
// parse requests of content-type - application/x-www-form-urlencoded
app.use(express.urlencoded({ extended: true }));

userRouter(app);
labTaskRouter(app);

const PORT = process.env.PORT || 8080;
app.listen(PORT, () => {
    logger.info(`Server is running on port ${PORT}.`);
});

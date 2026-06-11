const { Matrix, OLS, trainTestSplit, metrics, addConstantVariable } = require('bindings')('ml');
const { correlation } = metrics.dataset;

const csv = require('@fast-csv/parse');

const dataArray = [];

csv.parseFile('datasets/housing.csv', { headers: true })
    .on('error', error => console.error(error))
    .on('data', row => {
        // dataArray.push(row);

        let matrixRow = [];

        matrixRow.push(
            parseFloat(row.longitude),
            parseFloat(row.latitude),
            parseFloat(row.housing_median_age),
            parseFloat(row.total_rooms/* / row.households*/),
            parseFloat(row.total_bedrooms), // EXCLUDED
            parseFloat(row.households),
            parseFloat(row.population/* / row.households*/),
            parseFloat(row.median_income),
        );

        if (row.ocean_proximity === 'NEAR BAY') {
            matrixRow.push(1, 0, 0/*, 0*/);
        }
        else if (row.ocean_proximity === 'NEAR OCEAN' || row.ocean_proximity === 'ISLAND') {
            matrixRow.push(0, 1, 0/*, 0*/);
        }
        else if (row.ocean_proximity === '<1H OCEAN') {
            matrixRow.push(0, 0, 1/*, 0*/);
        }
        // else if (row.ocean_proximity === 'ISLAND') {
        //     matrixRow.push(0, 0, 0, 1);
        // }
        else {
            matrixRow.push(0, 0, 0/*, 0*/);
        }

        matrixRow.push(parseFloat(row.median_house_value));

        dataArray.push(matrixRow);
    })
    .on('end', rowCount => {
        console.log(`Parsed ${rowCount} rows`);

        let data = Matrix.fromArray(dataArray);

        const lastColumnIndex = data.getColCount() - 1;
        let X = data.project({ excludeCols: [4, lastColumnIndex] });
        let y = data.project({ cols: [lastColumnIndex] });

        console.log(correlation(X).toArray().map(row => row.map(val => Math.round(val * 100) / 100)));

        X = addConstantVariable(X);

        // console.log(X.toArray())

        let { xTrain, xTest, yTrain, yTest } = trainTestSplit({
            X, y, testSize: 0.2, seed: 1234
        });


        const ols = new OLS();

        ols.fit(xTrain, yTrain);

        const id = Matrix.identity(lastColumnIndex);

        console.log(ols.predict(id).toArray())

        let pred = ols.predict(xTest);
        let diff = pred.subtract(yTest);
        let mse = diff
            .transpose()
            .multiply(diff)
            .multiplyBy(1 / diff.getRowCount()).toArray()

        // console.log(mse);

        console.log(metrics.regression.meanSquaredError(yTest, pred));
        console.log(metrics.regression.meanAbsoluteError(yTest, pred));
        console.log(metrics.regression.rSquared(yTest, pred));

        // 4775782912
        // 49606.1953125
        // 0.6382367610931396

    });


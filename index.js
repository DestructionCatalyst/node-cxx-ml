const addon = require('bindings')('ml');

const { Matrix, OLS } = addon;

// console.log(addon.Matrix)

console.log(new addon.Matrix(2, 3).getRowCount());
console.log(new addon.Matrix(2, 3).getColCount());
console.log(addon.Matrix.zeros(2, 3).getRowCount());
console.log(addon.Matrix.zeros(2, 3).getColCount());
console.log(addon.Matrix.identity(5).getRowCount());
console.log(addon.Matrix.identity(5).getColCount());

console.log(new addon.Matrix(3, 2).toArray());
console.log(addon.Matrix.zeros(5, 10).toArray());
console.log(addon.Matrix.identity(5).toArray());

const a = addon.Matrix.fromArray([
    [1, 2, 3],
    [3, 4, 5],
    [5, 6, 7],
]);

console.log(a.toArray());

const b = addon.Matrix.identity(3);

const sum = a.add(b);

console.log(sum.toArray());

const diff = a.subtract(b);

console.log(diff.toArray());

const prod1 = diff.multiplyBy(2);

console.log(prod1.toArray());

const c = addon.Matrix.fromArray([
    [1, 2, 3, 4, 5],
    [3, 4, 5, 6, 0.7],
    [5, 6, 7, 8.1, 9],
]);

const prod2 = c.multiplyBy(0.2);

console.log(prod2.toArray());

const prod3 = a.multiply(b);

console.log(prod3.toArray());

const prod4 = sum.multiply(c);

console.log(prod4.toArray());

const t1 = a.transpose();

console.log(t1.toArray());

const t2 = c.transpose();

console.log(t2.toArray());

const tm = c.multiply(c.transpose());

console.log(tm.toArray());

console.log(tm.inverse().toArray());

const X = addon.Matrix.fromArray([
    [1, 1],
    [2, 1],
    [2.5, 1],
    [3, 1],
]);

const y = addon.Matrix.fromArray([
    [4],
    [6],
    [6],
    [7],
]);

// console.log(X.transpose().multiply(X).toArray())
console.log((X.transpose().multiply(X).inverse().multiply(X.transpose())).multiply(y).toArray());

const ols = new OLS();

ols.fit(X, y);

const demoMatrix = Matrix.fromArray([
    [1, 0],
    [0, 1],
]);

console.log(ols.predict(demoMatrix).toArray());

console.log(c.toArray())
console.log(c.project({ rows: [0, 1, 2], cols: [0, 1, 2, 3, 4] }).toArray())
console.log(c.project({ rows: [0, 1], cols: [1, 2] }).toArray())
console.log(c.project({ rows: [0, 1] }).toArray())
console.log(c.project({ cols: [3, 4] }).toArray())

console.log(c.project({ excludeRows: [0] }).toArray())
console.log(c.project({ excludeCols: [2, 3] }).toArray())
console.log(c.project({ excludeRows: [0], cols: [1, 2] }).toArray())
console.log(c.project({ rows: [0, 2], excludeCols: [2, 3] }).toArray())
console.log(c.project({ excludeRows: [0, 2], excludeCols: [2, 3] }).toArray())
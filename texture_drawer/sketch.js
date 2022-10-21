const $ = s => document.querySelector(s);
const $$ = s => document.querySelectorAll(s);

const root = $('#root');
const modeBtn = $('#modeBtn');
let WIDTH = 16;
let HEIGHT = 20;
let editMode = 'draw';
let mouseIsDown = false;

function createCells() {
	root.innerHTML = '';

	for (let i = 0; i < HEIGHT; i++) {
		const row = document.createElement('tr');
		row.classList.add('row');

		for (let j = 0; j < WIDTH; j++) {
			const cell = document.createElement('th');
			cell.classList.add('cell');
			row.appendChild(cell);
		}

		root.appendChild(row);
	}
}

function drawPixel(e) {
	const { clientX, clientY } = e;
	const { x, y } = root.children[0].getBoundingClientRect();
	const { height } = root.children[0].children[0].getBoundingClientRect();

	if ((clientX > x) && (clientX < x + height * WIDTH) && (clientY > y) && (clientY < y + height * HEIGHT)) {
		const row = Math.floor((clientY - y) / height);
		const col = Math.floor((clientX - x) / height);

		const cell = root.children[row].children[col];
		if (editMode === 'draw') cell.classList.add('filled');
		else cell.classList.remove('filled');
	}
}

window.addEventListener('mousedown', e => {
	if (e.target !== 'submit') {
		mouseIsDown = true;
		drawPixel(e);
	}
});

window.addEventListener('mouseup', () => {
	mouseIsDown = false;
});

window.addEventListener('mousemove', e => {
	if (mouseIsDown) drawPixel(e);
});

modeBtn.addEventListener('click', () => {
	if (editMode === 'draw') {
		editMode = 'erase';
		modeBtn.textContent = 'Erase';
		document.body.classList.add('erase')
	}

	else {
		editMode = 'draw';
		modeBtn.textContent = 'Draw';
		document.body.classList.remove('erase');
	}
});

$('#sizesBtn').addEventListener('click', () => {
	const width = parseInt(prompt('Enter width:'));
	const height = parseInt(prompt('Enter height:'));

	WIDTH = width;
	HEIGHT = height;
	createCells();
});

$('#clearBtn').addEventListener('click', () => {
	$$('.cell.filled').forEach(cell => {
		cell.classList.remove('filled');
	})
});

$('#exportBtn').addEventListener('click', () => {
	let matrix = [];
	let lastWasFilled = false;
	let pixelsInRow = 0;

	for (const row of root.children) {
		for (const cell of row.children) {
			const isFilled = cell.classList.contains('filled');

			if (isFilled === lastWasFilled) {
				pixelsInRow++;
			} else {
				matrix.push(pixelsInRow);
				pixelsInRow = 1;
			}

			lastWasFilled = isFilled;
		}
	}

	matrix.push(pixelsInRow);
	console.log(matrix);
});

createCells();
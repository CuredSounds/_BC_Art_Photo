const canvas = document.getElementById('canvas');
const kRedWeight = 0.299;
const kGreenWeight = 0.587;
const kBlueWeight = 0.114;

const context = canvas.getContext('2d');
context.lineCap = 'round';

const state = {
  tool: 'brush',
  color: '#000000',
  size: 5,
  drawing: false,
  startX: 0,
  startY: 0,
  snapshot: null,
  brightness: 100,
  contrast: 100,
  saturation: 100,
};

function applyFilters() {
  canvas.style.filter = `brightness(${state.brightness}%) contrast(${state.contrast}%) saturate(${state.saturation}%)`;
}

function drawLine(x1, y1, x2, y2) {
  context.save();
  context.lineWidth = state.size;
  context.strokeStyle = state.tool === 'eraser' ? '#ffffff' : state.color;
  context.beginPath();
  context.moveTo(x1, y1);
  context.lineTo(x2, y2);
  context.stroke();
  context.restore();
}

function getCanvasCoordinates(event) {
  const rect = canvas.getBoundingClientRect();
  return {
    x: event.clientX - rect.left,
    y: event.clientY - rect.top,
  };
}

canvas.addEventListener('mousedown', (event) => {
  const { x, y } = getCanvasCoordinates(event);
  state.drawing = true;
  state.startX = x;
  state.startY = y;
  state.snapshot = context.getImageData(0, 0, canvas.width, canvas.height);

  if (state.tool === 'brush' || state.tool === 'eraser') {
    drawLine(x, y, x, y);
  }
});

canvas.addEventListener('mousemove', (event) => {
  if (!state.drawing) {
    return;
  }

  const { x, y } = getCanvasCoordinates(event);

  if (state.tool === 'brush' || state.tool === 'eraser') {
    drawLine(state.startX, state.startY, x, y);
    state.startX = x;
    state.startY = y;
    return;
  }

  context.putImageData(state.snapshot, 0, 0);
  context.lineWidth = state.size;
  context.strokeStyle = state.color;

  if (state.tool === 'line') {
    context.beginPath();
    context.moveTo(state.startX, state.startY);
    context.lineTo(x, y);
    context.stroke();
    return;
  }

  if (state.tool === 'rectangle') {
    context.strokeRect(state.startX, state.startY, x - state.startX, y - state.startY);
  }
});

window.addEventListener('mouseup', () => {
  state.drawing = false;
});

document.getElementById('tool').addEventListener('change', (event) => {
  state.tool = event.target.value;
});

document.getElementById('color').addEventListener('input', (event) => {
  state.color = event.target.value;
});

document.getElementById('size').addEventListener('input', (event) => {
  state.size = Number(event.target.value);
});

document.getElementById('brightness').addEventListener('input', (event) => {
  state.brightness = Number(event.target.value);
  applyFilters();
});

document.getElementById('contrast').addEventListener('input', (event) => {
  state.contrast = Number(event.target.value);
  applyFilters();
});

document.getElementById('saturation').addEventListener('input', (event) => {
  state.saturation = Number(event.target.value);
  applyFilters();
});

document.getElementById('grayscale').addEventListener('click', () => {
  const image = context.getImageData(0, 0, canvas.width, canvas.height);
  const { data } = image;
  for (let i = 0; i < data.length; i += 4) {
    const gray = kRedWeight * data[i] + kGreenWeight * data[i + 1] + kBlueWeight * data[i + 2];
    data[i] = gray;
    data[i + 1] = gray;
    data[i + 2] = gray;
  }
  context.putImageData(image, 0, 0);
});

document.getElementById('invert').addEventListener('click', () => {
  const image = context.getImageData(0, 0, canvas.width, canvas.height);
  const { data } = image;
  for (let i = 0; i < data.length; i += 4) {
    data[i] = 255 - data[i];
    data[i + 1] = 255 - data[i + 1];
    data[i + 2] = 255 - data[i + 2];
  }
  context.putImageData(image, 0, 0);
});

document.getElementById('upload').addEventListener('change', (event) => {
  const [file] = event.target.files;
  if (!file) {
    return;
  }

  const reader = new FileReader();
  reader.onload = () => {
    const image = new Image();
    image.onload = () => {
      context.clearRect(0, 0, canvas.width, canvas.height);
      context.drawImage(image, 0, 0, canvas.width, canvas.height);
    };
    image.src = reader.result;
  };
  reader.readAsDataURL(file);
});

document.getElementById('save').addEventListener('click', () => {
  const link = document.createElement('a');
  link.download = 'bc-art-photo.png';
  link.href = canvas.toDataURL('image/png');
  link.click();
});

document.getElementById('reset').addEventListener('click', () => {
  state.brightness = 100;
  state.contrast = 100;
  state.saturation = 100;
  document.getElementById('brightness').value = '100';
  document.getElementById('contrast').value = '100';
  document.getElementById('saturation').value = '100';
  applyFilters();
});

context.fillStyle = '#ffffff';
context.fillRect(0, 0, canvas.width, canvas.height);

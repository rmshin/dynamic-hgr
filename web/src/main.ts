import type { Category, GestureRecognizerResult } from '@mediapipe/tasks-vision';
import { GestureRecognizer, FilesetResolver, DrawingUtils } from '@mediapipe/tasks-vision';

let gestureRecognizer: GestureRecognizer;
let runningMode = 'VIDEO';
let enableWebcamButton: HTMLButtonElement;
const width = 480;
const height = 360;
const canvasWidth = window.devicePixelRatio * width;
const canvasHeight = window.devicePixelRatio * height;
const videoHeight = `${height}px`;
const videoWidth = `${width}px`;

// Before we can use HandLandmarker class we must wait for it to finish
// loading. Machine Learning models can be large and take a moment to
// get everything needed to run.
const createGestureRecognizer = async () => {
  const vision = await FilesetResolver.forVisionTasks(
    'https://cdn.jsdelivr.net/npm/@mediapipe/tasks-vision@0.10.3/wasm'
  );
  gestureRecognizer = await GestureRecognizer.createFromOptions(vision, {
    baseOptions: {
      modelAssetPath: '/src/model/gesture_recognizer.task',
      delegate: 'GPU',
    },
    runningMode: runningMode,
    numHands: 2,
  });
};
createGestureRecognizer();

/********************************************************************
  // Demo 2: Continuously grab image from webcam stream and detect it.
  ********************************************************************/

const video = document.getElementById('webcam_feed') as HTMLVideoElement;
const canvasElement = document.getElementById('output_canvas') as HTMLCanvasElement;
canvasElement.width = canvasWidth;
canvasElement.height = canvasHeight;
const canvasCtx = canvasElement.getContext('2d') as CanvasRenderingContext2D;
const gestureOutput = document.getElementById('gesture_output') as HTMLElement;

// Check if webcam access is supported.
function hasGetUserMedia() {
  return !!(navigator.mediaDevices && navigator.mediaDevices.getUserMedia);
}

// If webcam supported, add event listener to button for when user
// wants to activate it.
if (hasGetUserMedia()) {
  enableWebcamButton = document.getElementById('webcamButton') as HTMLButtonElement;
  enableWebcamButton.addEventListener('click', enableCam);
} else {
  console.warn('getUserMedia() is not supported by your browser');
}

// Enable the live webcam view and start detection.
function enableCam(_: Event) {
  if (!gestureRecognizer) {
    alert('Please wait for gestureRecognizer to load');
    return;
  }

  // Activate the webcam stream.
  navigator.mediaDevices.getUserMedia({ video: true }).then(function (stream) {
    video.srcObject = stream;
    video.addEventListener('loadeddata', predictWebcam);
  });
}

let lastVideoTime = -1;
let results: GestureRecognizerResult | undefined = undefined;
async function predictWebcam() {
  // Now let's start detecting the stream.
  let nowInMs = Date.now();
  if (video.currentTime !== lastVideoTime) {
    lastVideoTime = video.currentTime;
    results = gestureRecognizer.recognizeForVideo(video, nowInMs);
  }

  canvasCtx.save();
  canvasCtx.clearRect(0, 0, canvasElement.width, canvasElement.height);
  const drawingUtils = new DrawingUtils(canvasCtx);

  canvasElement.style.height = videoHeight;
  video.style.height = videoHeight;
  canvasElement.style.width = videoWidth;
  video.style.width = videoWidth;

  if (results?.landmarks) {
    for (const landmarks of results.landmarks) {
      drawingUtils.drawConnectors(landmarks, GestureRecognizer.HAND_CONNECTIONS, {
        color: '#00FF00',
        lineWidth: 5,
      });
      drawingUtils.drawLandmarks(landmarks, {
        color: '#FF0000',
        lineWidth: 2,
      });
    }
  }
  canvasCtx.restore();
  if (results && results.gestures.length > 0) {
    gestureOutput.style.display = 'block';
    gestureOutput.style.width = videoWidth;
    let predictionText = '';
    results.gestures.forEach((gestures, idx) => {
      const gesture: Category = gestures[0];
      const categoryName = gesture.categoryName;
      const categoryScore = (gesture.score * 100).toFixed(2);
      const handedness = results.handednesses[idx][0].displayName;
      predictionText += `GestureRecognizer: ${categoryName}\n Confidence: ${categoryScore} %\n Handedness: ${handedness}\n`;
    });
    gestureOutput.innerText = predictionText;
  } else {
    gestureOutput.style.display = 'none';
  }
  // Call this function again to keep predicting when the browser is ready.
  window.requestAnimationFrame(predictWebcam);
}

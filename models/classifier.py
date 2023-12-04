import torch
import random, time
import torch.nn as nn


### Simple 3-layer fully-connected model
class GestureClassifier(nn.Module):
    def __init__(self, num_hidden=50, num_gestures=10):
        super().__init__()
        self.net = nn.Sequential(
            # 21 landmarks * 3 coords per landmark * 2 hands
            nn.Linear(21 * 3 * 2, num_hidden),
            nn.BatchNorm1d(num_hidden),
            nn.ReLU(),
            nn.Linear(num_hidden, num_hidden),
            nn.BatchNorm1d(num_hidden),
            nn.ReLU(),
            nn.Linear(num_hidden, num_gestures),
            nn.BatchNorm1d(num_gestures),
        )

    def forward(self, x):
        return self.net(x)

    def classify(self, x):
        logits = self(x)
        probs = nn.functional.softmax(logits, dim=-1)
        classification = torch.argmax(probs)
        return classification


### Training
with open("landmarks.csv", "r") as f:
    rows = f.read().splitlines()
    random.shuffle(rows)
    Xs = [[float(coord) for coord in row.split(",")[1:]] for row in rows]
    Ys = [int(index) for row in rows for index in row.split(",", 1)[0]]
    Xs = torch.tensor(Xs)
    Ys = torch.tensor(Ys)

n = int(len(Xs) * 0.9)
Xtr, Ytr = Xs[:n], Ys[:n]
Xval, Yval = Xs[n:], Ys[n:]

device = "cuda" if torch.cuda.is_available() else "cpu"
learning_rate = 1e-3
classifier = GestureClassifier().to(device)
optimiser = torch.optim.SGD(classifier.parameters(), lr=learning_rate)
print(
    "Model size: ",
    sum(p.numel() for p in classifier.parameters()) / 1e3,
    "K parameters\n",
)

start = time.perf_counter()
for i in range(20000):
    optimiser.zero_grad()
    output = classifier(Xtr)
    loss = nn.functional.cross_entropy(output, Ytr)
    loss.backward()
    optimiser.step()
    if (i + 1) % 1000 == 0:
        print(f"progress: {(i+1) / 20000 * 100:.2f}%")

end = time.perf_counter()
elapsed = end - start
hours = int(elapsed // 3600)
minutes = int((elapsed % 3600) // 60)
seconds = int(elapsed % 60)
print(f"Total elapsed time: {hours:02d}:{minutes:02d}:{seconds:02d}")


@torch.no_grad()
def split_loss(split):
    classifier.eval()
    x, y = {"train": (Xtr, Ytr), "val": (Xval, Yval)}[split]
    y_hat = classifier(x)
    loss = nn.functional.cross_entropy(y_hat, y)
    classifier.train()
    print(split, loss.item())


split_loss("train")
split_loss("val")

# export trained model to serialised format for C++ consumption
# force model to run on cpu because mediapipe gpu support is linux-only
script_module = torch.jit.script(classifier.to("cpu"))
script_module.save("gesture_classifier.pt")

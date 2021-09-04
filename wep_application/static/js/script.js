var dropbox = document.getElementById("dropbox");
var spinner = document.getElementById("spinner");
var solveBtn = document.getElementById("solveBtn");
var homeBtn = document.getElementById("homeBtn");
var prevBtn = document.getElementById("prevBtn");
var nextBtn = document.getElementById("nextBtn");
var dwnlBtn = document.getElementById("dwnlBtn");
var statusText = document.getElementById("statusText");
var gallery = document.getElementById("gallery");
var ordrImg = document.getElementById("ordrImg");
var dtstr;
var imgId;
var timgs;
var us;

dropbox.addEventListener("dragenter", dragenter, false);
dropbox.addEventListener("dragover", dragover, false);
dropbox.addEventListener("dragleave", dragleave, false);
dropbox.addEventListener("drop", drop, false);

solveBtn.addEventListener("click",solveClick);
prevBtn.addEventListener("click",prevClick);
nextBtn.addEventListener("click",nextClick);
homeBtn.addEventListener("click",homeClick);


function dragenter(e) {
    e.stopPropagation();
    e.preventDefault();
}

function dragover(e) {
    e.stopPropagation();
    e.preventDefault();
    dropbox.style.backgroundColor = "black";
}

function dragleave(e) {
    e.stopPropagation();
    e.preventDefault();
    dropbox.style.backgroundColor = '';
}

function drop(e) {
    e.stopPropagation();
    e.preventDefault();

    const dt = e.dataTransfer;
    const files = dt.files;
    const form = new FormData();

    form.append("file", files[0]);

    fetch("upload", {
        method: "post",
        body: form
    }).catch(console.error);

    console.log(files);

    dropbox.style.backgroundColor = '';
    dropbox.innerText = files[0].name;
    solveBtn.disabled = false;
}

function solveClick() {
    fetch("solve");
    dropbox.style.display = "none";
    solveBtn.style.display = "none";
    solveBtn.disabled = true;
    spinner.style.display = "inline-block";
    statusText.style.display = "block";
    statusText.innerText = "Waiting...";

    us = setInterval(updateStatus,250);
}

function prevClick() {
    if (imgId==timgs) nextBtn.disabled = false;
    ordrImg.src = `./static/img/${dtstr}${--imgId}.png`;
    if (imgId==1) prevBtn.disabled = true;
}

function nextClick() {
    if (imgId==1) prevBtn.disabled = false;
    ordrImg.src = `./static/img/${dtstr}${++imgId}.png`;
    if (imgId==timgs) nextBtn.disabled = true;
}

function homeClick() {
    homeBtn.style.display = "none";
    prevBtn.style.display = "none";
    nextBtn.style.display = "none";
    dwnlBtn.style.display = "none";
    gallery.style.display = "none";
    statusText.style.display = "none";
    spinner.style.display = "none";

    dropbox.style.display = "inline-block";
    solveBtn.style.display = "inline-block";
    dropbox.innerText = "Drop \".hcp\" file here.";
    solveBtn.disabled = true;
}

function updateStatus() {
    fetch("status").then(response => {
        return response.json().then(statusJson => {
            status = statusJson.status;
            console.log(status);
            switch (status) {
                case "1":
                    statusText.innerText = "Parsing the graph file...";
                    break;
                case "2":
                    let vertices = Number(statusJson.vertices);
                    let snakes = statusJson.stage!="0"  ? vertices - Number(statusJson.gaps) : "?";
                    statusText.innerText = `Solving: Stage ${statusJson.stage}\t-\t${snakes}/${vertices} snakes`;
                    break;
                case "3":
                    statusText.innerText = `Generating images: ${statusJson.imgs}/${statusJson.timgs}`;
                    break;
                case "4":
                    clearInterval(us);
                    timgs = statusJson.timgs;
                    dtstr = statusJson.dtstr;
                    statusText.innerText = `Finished in ${statusJson.time} seconds.`;
                    setTimeout(setupGallery,1500);
                    break;
            }
        })
    })
}

function setupGallery() {
    spinner.style.display = "none";
    statusText.style.display = "none";
    homeBtn.style.display = "inline-block";
    prevBtn.style.display = "inline-block";
    nextBtn.style.display = "inline-block";
    dwnlBtn.style.display = "inline-block";
    gallery.style.display = "inline-block";
    ordrImg.src = `./static/img/${dtstr}1.png`;
    imgId = 1;
    prevBtn.disabled = true;
    nextBtn.disabled = false;
}

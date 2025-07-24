class Subject {
  constructor(name, mark) {
    this.subject = name;
    this.mark = mark;
  }
}

class Block {
  constructor(studentId, semester, subjects, prevHash = "0") {
    this.student_id = studentId;
    this.semester = semester;
    this.subjects = subjects;
    this.subject_count = subjects.length;
    this.timestamp = new Date().toISOString();
    this.prev_hash = prevHash;
    this.hash = ""; 
    this.signature = ""; 
  }
}

let chain = [];


function computeHash(block) {
  const data = block.student_id + block.semester + JSON.stringify(block.subjects) + block.prev_hash + block.timestamp;
  return CryptoJS.SHA256(data).toString(); // Requires crypto-js
}


function addBlock(studentId, semester, subjectList) {
  document.getElementById('output').innerHTML='<h2>ADDING BLOCK</h2>';

  const subjects = subjectList.map(sub => new Subject(sub.name, sub.mark));
  const prevHash = chain.length === 0 ? "0" : chain[chain.length - 1].hash;

  const newBlock = new Block(studentId, semester, subjects, prevHash);
  newBlock.hash = computeHash(newBlock);

  chain.push(newBlock);
  console.log("✅ Block added:", newBlock);

}

addBlock("1rv23cy4020",6,)


function displayBlocks() {
      document.getElementById('output').innerHTML = '<h2>Display all current blocks</h2>';
}


function verifyChain() {
  document.getElementById('output').innerHTML = '<h2>Verifying full chain...</h2>';
}


function verifyBlock() {
  document.getElementById('output').innerHTML = '<h2>Verify individual block</h2>';
}


function saveToJson() {
  document.getElementById('output').innerHTML = '<h2>Saving blocks to JSON...</h2>';
}


function displayFromJson() {
  document.getElementById('output').innerHTML = '<h2>Displaying blocks from JSON</h2> This is a test';
  fetch('../db/demo/personal.json')
    .then(response => response.json())
    .then(data=>{
      const output=document.getElementById('output');
      let html=`<h3>Student ID: ${data.student_id}</h3>`;
      html+=`<h3>Semester: ${data.semester}</h3>`;
      html+='<h3>Subjects:</h3>ul';
      data.subjects.forEach()
    })
}


function verifyFromJson() {
  document.getElementById('output').innerHTML = '<h2>Verifying JSON file blocks...</h2>';
}


function aboutUS() {
  document.getElementById('output').innerHTML = '<h2>Created by XYZ for academic blockchain demo.</h2>';

}

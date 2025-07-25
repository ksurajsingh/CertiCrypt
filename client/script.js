document.addEventListener("DOMContentLoaded", () => {
  displayFromJson();
});

class Subject {
  constructor(name, mark) {
    this.subject = name;
    this.mark = mark;
  }
}

class Block {
  constructor(studentId, semester, subjects, subject_count, timestamp, prev_hash, hash, signature){
  // constructor(studentId, semester, subjects, prevHash = "0") {
    this.student_id = studentId;
    this.semester = semester;
    this.subjects = subjects;
    this.subject_count = subject_count;
    this.timestamp = timestamp;
    this.prev_hash = prev_hash;
    this.hash = hash; 
    this.signature = signature; 
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

  const output=document.getElementById('output');
  output.innerHTML = '<h2>Displaying blocks from JSON</h2>';
  const chain=document.createElement('div');
  chain.classList.add('chain');

  // fetching json data
  fetch('http://localhost:8080/db/demo/personal.json')
    .then(response => response.text())
    .then(text=>{

      // processing json data
      console.log("received json\n"+text);
      const lines=text.trim().split('\n');
      const jsonData=lines.map(line=>JSON.parse(line));
      console.log("processed json\n"+jsonData);
      console.log(jsonData);
      const blocks=lines.map(line=>{
        const obj=JSON.parse(line);
        return new Block(
          obj.student_id,
          obj.semester,
          obj.subjects,
          obj.subject_count,
          obj.timestamp,
          obj.prev_hash,
          obj.hash,
          obj.signature
        );
      });

      // displaying the json data
      console.log(blocks);
      blocks.forEach(block=>{
        console.log(block);
        blockHTML=document.createElement('div')
        blockHTML.classList.add('block');
        blockHTML.innerHTML=`
        <div class='blockE'><span class='key'>Student_id: </span><span class='value'>${block.student_id}</span></div>
        <div class='blockE'><span class='key'>Semester: </span><span class='value'>${block.semester}</span></div>
        <div class='blockE subs'><span class='key'>Subjects: </span><span class='value'>
          ${block.subjects.map(sub=>(`<div class='subE'>
              <span class='key'>${sub.subject}: <span><span class='value'>${sub.mark}<span>
              </div>`
          )).join('')}
          </span></div>
        <div class='blockE'><span class='key'>Subject_count: </span><span class='value'>${block.subject_count}</span></div>
        <div class='blockE'><span class='key'>Timestamp: </span><span class='value'>${block.timestamp}</span></div>
        <div class='blockE'><span class='key'>Prev_Hash: </span><span class='value'>${block.prev_hash}</span></div>
        <div class='blockE'><span class='key'>Hash: </span><span class='value'>${block.hash}</span></div>
        <div class='blockE'><span class='key'>Signature: </span><span class='value'>${block.signature}</span></div>
        `
        chain.appendChild(blockHTML);
      })

    })
   
  output.appendChild(chain);

}


function verifyFromJson() {
  const output=document.getElementById('output');
  output.innerHTML = '<h2>Verifying JSON file blocks...</h2>';
  const chain=document.createElement('div');
  chain.classList.add('chain');


 // fetching json data
  fetch('http://localhost:8080/db/demo/personal.json')
    .then(response => response.text())
    .then(text=>{

      // processing json data
      // console.log("received json\n"+text);
      const lines=text.trim().split('\n');
      // const jsonData=lines.map(line=>JSON.parse(line));
      // console.log("processed json\n"+jsonData);
      // console.log(jsonData);
      const blocks=lines.map(line=>{
        const obj=JSON.parse(line);
        return new Block(
          obj.student_id,
          obj.semester,
          obj.subjects,
          obj.subject_count,
          obj.timestamp,
          obj.prev_hash,
          obj.hash,
          obj.signature
        );
      });


      blocks.forEach(block=>{
        // console.log(block);
        blockHTML=document.createElement('div')
        block.element=blockHTML;
        blockHTML.classList.add('block');
        blockHTML.innerHTML=`
        <div class='blockE'><span class='key'>Student_id: </span><span class='value'>${block.student_id}</span></div>
        <div class='blockE'><span class='key'>Semester: </span><span class='value'>${block.semester}</span></div>
        <div class='blockE subs'><span class='key'>Subjects: </span><span class='value'>
          ${block.subjects.map(sub=>(`<div class='subE'>
              <span class='key'>${sub.subject}: <span><span class='value'>${sub.mark}<span>
              </div>`
          )).join('')}
          </span></div>
        <div class='blockE'><span class='key'>Subject_count: </span><span class='value'>${block.subject_count}</span></div>
        <div class='blockE'><span class='key'>Timestamp: </span><span class='value'>${block.timestamp}</span></div>
        <div class='blockE'><span class='key'>Prev_Hash: </span><span class='value'>${block.prev_hash}</span></div>
        <div class='blockE'><span class='key'>Hash: </span><span class='value'>${block.hash}</span></div>
        <div class='blockE'><span class='key'>Signature: </span><span class='value'>${block.signature}</span></div>
        `
        chain.appendChild(blockHTML);


     


    })

      for(i=1;i<blocks.length;i++){
      if(blocks[i].prev_hash!== blocks[i-1].hash){
        const elem=blocks[i-1].element;
        elem.style.backgroundColor="red";
      }
      }

  output.appendChild(chain);
  
})
}


function aboutUS() {
  document.getElementById('output').innerHTML = '<h2>Created by XYZ for academic blockchain demo.</h2>';

}

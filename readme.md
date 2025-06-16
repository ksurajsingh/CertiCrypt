# TODO 

## Phase - 1 
- [x] Block Structure with 
      - student ID 
      - semester,subjects & marks 
      - Timestamp
      - PreviousHash
- [ ] SHA-256 hash chaining 
- [ ] Digital Signatures
- [ ] CLI to 
      - Add a new block 
      - verify chain integrity
      - verify signature 
- [ ] Json file storage 

## Phase - 2 

- [ ] Merkle Tree
      - Hash subject-mark pair
      - Store merkle root in block 
      - Support merkle proof generation and verification 
- [ ] Tamper Detection 
      - Validate full chain + merkle integrity
      - CLI to check student's markscard integrity
- [ ] ZKP-like logic (simulated)
      - GPA > X show proof without revealing all marks 
      - Use selective disclose or dummy logic 
- [ ] GUI (Tkinter) or web(Flask) frontend (if time permits) 

## Deliverables 

- [ ] code 
- [ ] CLI or GUI app
- [ ] sample data (demo markscard)
- [ ] Screenshots + README
- [ ] Short report (explain structure + feature)

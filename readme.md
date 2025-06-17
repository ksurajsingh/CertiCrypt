# TODO 

Its currently considered that the last block isn't verified

## Phase - 1 
- [x] Block Structure with   
      - student ID   
      - semester,subjects & marks  
      - Timestamp  
      - PreviousHash  
- [x] SHA-256 hash chaining 
      - [x] Chain verification   
      - [x] Block verification  
- [ ] Digital Signatures
- [ ] CLI to **[ Started ]**    
      - Add a new block   
      - verify chain integrity  
      - verify signature   
- [ ] Json file storage  [ store hashes as well - 2 step verification (this and the verificatoin using ```prev_hash``` ) ]  

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
- [ ] User restrictive priveleges - the data nor the hash should be modified . [ at certain intervals the hash of the blocks should be updated and verified. ]  

## Deliverables 

- [ ] code 
- [ ] CLI or GUI app  
- [ ] sample data (demo markscard)  
- [ ] Screenshots + README  
- [ ] Short report (explain structure + feature)  

## Experimental 

- [ ] consensus  [ multiple chains]  
- [ ] See if you can generate the hash for the whole data structure of the block instead of using another instance of *character*  
      - reduces code complexity    
      - increase code readability  

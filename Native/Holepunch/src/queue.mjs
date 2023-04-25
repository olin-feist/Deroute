//FIFO queue for storing propogated searches
class Queue {
    que = []
    max_length = 64

    //discards First In if full
    push (obj){
      if (this.que.length >= this.max_length){
        this.que.shift()
      }
      this.que.push(obj)
    }
  
    contains (obj){
      var exists = false
      this.que.forEach(element => {
        if (element === obj) exists = true
      })
      return exists
    }
  }

  export default Queue;
LetterToHexDict={
    "a":"A",
    "b":"B",
    "c":"C",
    "d":"D",
    "e":"E",
    "f":"F",
    "g":"6",
    "h":None,
    "i":"1",
    "j":None,
    "k":None,
    "l":"1",
    "m":None,
    "n":None,
    "o":"0",
    "p":None,
    "q":None,
    "r":None,
    "s":"5",
    "t":None,
    "u":None,
    "v":None,
    "w":None,
    "x":None,
    "y":None,
    "z":"2",
}

def SpellInHex(word:str):
    NewSpell=""
    for Char in word:
        if (LetterToHexDict.get(Char,None)):
            NewSpell+=LetterToHexDict[Char]
        else:
            return False
    return NewSpell

with open("wordlist.txt") as Stringer:
    for x in Stringer:
        ret=SpellInHex(x.strip().lower())
        if ret:
            print(f"{x}:{ret}")
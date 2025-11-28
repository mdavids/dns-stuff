# Some DNS experiments

## mini-dig

PoC to display TXT records as UTF-8 (Golang's `net.LookupTXT` does this out of the box).

~~~
go build ./mini-dig.go
./mini-dig utf-8.testdns.nl

; <<>> Go TXT Lookup Tool - dig-like utility <<>>
; Searching for TXT records for: utf-8.testdns.nl
;; QUESTION SECTION:
;utf-8.testdns.nl.		IN	TXT

;; ANSWER SECTION:
utf-8.testdns.nl.		IN	TXT	"ᑖᓐᓇ ᐱᐅᔪᒻᒪᕆᐅᔮᖅᑐᖅ."
utf-8.testdns.nl.		IN	TXT	"𝒯𝒽𝒾𝓈 𝓁ℴℴ𝓀𝓈 𝓅𝓇ℯ𝓉𝓉𝓎 𝓃𝒾𝒸ℯ."
utf-8.testdns.nl.		IN	TXT	"https://テスト.XN--ZCKZAH/"
utf-8.testdns.nl.		IN	TXT	"https://example.com.ו.קום/ـ/"
utf-8.testdns.nl.		IN	TXT	"mailto:🤓@example.テスト"
utf-8.testdns.nl.		IN	TXT	"𝚃𝚑𝚒𝚜 𝚕𝚘𝚘𝚔𝚜 𝚙𝚛𝚎𝚝𝚝𝚢 𝚗𝚒𝚌𝚎."
utf-8.testdns.nl.		IN	TXT	"Ça a l'air plutôt bien."
utf-8.testdns.nl.		IN	TXT	"𓀐𓂺"
utf-8.testdns.nl.		IN	TXT	"ִֶָ𓂃 ࣪˖ ִֶָ🐇་༘࿐"
utf-8.testdns.nl.		IN	TXT	"זה נראה די נחמד."
utf-8.testdns.nl.		IN	TXT	"かなり良さそうですね。"
utf-8.testdns.nl.		IN	TXT	"Pardon my French - I'm only testing some things here."
utf-8.testdns.nl.		IN	TXT	"ᶠᶸᶜᵏᵧₒᵤ!🖕"

;; Query completed.
~~~

Discussion on [DNS WG mailinglist here](https://mailarchive.ietf.org/arch/msg/dnsop/oNnBsRcb5L4z3m6__JlhxDfAJL0/).

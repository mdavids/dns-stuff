package main

import (
	"fmt"
	"net"
	"os"

	// Importeer de IDNA package
	"golang.org/x/net/idna"
)

func main() {
	// 1. Check if the user provided a domain name
	if len(os.Args) != 2 {
		fmt.Printf("Usage: %s <domain_name>\n", os.Args[0])
		os.Exit(1)
	}

	userInput := os.Args[1]

	// --- NIEUWE STAP: IDNA CONVERSIE ---
	// 2. Converteer de gebruikersinvoer (die Unicode kan bevatten, zoals 'bücher.de') 
	//    naar de Punycode (ASCII) representatie die vereist is voor DNS-lookups.
	domain, err := idna.ToASCII(userInput)
	if err != nil {
		fmt.Printf(";; ERROR: Invalid domain name or IDNA conversion failed for %s: %v\n", userInput, err)
		os.Exit(1)
	}
	// ------------------------------------

	fmt.Printf("; <<>> Go TXT Lookup Tool (IDN-compatible) <<>>\n")
	fmt.Printf("; Searching for TXT records for: %s (Punycode: %s)\n", userInput, domain)
	fmt.Printf(";; QUESTION SECTION:\n")
	fmt.Printf(";%s.\t\tIN\tTXT\n", domain) // Gebruik de geconverteerde 'domain' voor de lookup
	fmt.Printf("\n;; ANSWER SECTION:\n")

	// 3. Perform the TXT record lookup using the Punycode domain
	txtrecords, err := net.LookupTXT(domain)

	// 4. Handle errors
	if err != nil {
		fmt.Printf(";; ERROR: Could not retrieve TXT records for %s: %v\n", domain, err)
		os.Exit(1)
	}

	// 5. Print the found records
	if len(txtrecords) == 0 {
		fmt.Printf(";; No TXT records found for %s\n", domain)
	} else {
		for _, txt := range txtrecords {
			// Print in 'dig'-like style, using the PUNYCODE domain in the answer section
			fmt.Printf("%s.\t\tIN\tTXT\t\"%s\"\n", domain, txt)
		}
	}

	fmt.Printf("\n;; Query completed.\n")
}

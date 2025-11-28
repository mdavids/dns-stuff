package main

import (
	"fmt"
	"net"
	"os"
)

func main() {
	// 1. Check if the user provided a domain name as an argument
	if len(os.Args) != 2 {
		// Print usage message
		fmt.Printf("Usage: %s <domain_name>\n", os.Args[0])
		os.Exit(1)
	}

	// Get the domain name from the command-line arguments
	domain := os.Args[1]

	// Output section headers, similar to 'dig'
	fmt.Printf("; <<>> Go TXT Lookup Tool - dig-like utility <<>>\n")
	fmt.Printf("; Searching for TXT records for: %s\n", domain)
	fmt.Printf(";; QUESTION SECTION:\n")
	fmt.Printf(";%s.\t\tIN\tTXT\n", domain)
	fmt.Printf("\n;; ANSWER SECTION:\n")

	// 2. Perform the TXT record lookup using the standard net package
	txtrecords, err := net.LookupTXT(domain)

	// 3. Handle any errors during the lookup
	if err != nil {
		fmt.Printf(";; ERROR: Could not retrieve TXT records for %s: %v\n", domain, err)
		os.Exit(1)
	}

	// 4. Print the found records
	if len(txtrecords) == 0 {
		fmt.Printf(";; No TXT records found for %s\n", domain)
	} else {
		// Iterate through all found records and print them in 'dig'-like format
		for _, txt := range txtrecords {
			// Print in 'dig'-like style: domain.      IN      TXT     "record_value"
			fmt.Printf("%s.\t\tIN\tTXT\t\"%s\"\n", domain, txt)
		}
	}

	fmt.Printf("\n;; Query completed.\n")
}

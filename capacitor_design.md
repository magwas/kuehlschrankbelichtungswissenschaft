---
mainfont: DejaVu Serif
sansfont: DejaVu Sans
monofont: DejaVu Sans Mono
mathfont: DejaVu Math TeX Gyre
---
###  Geometry & Electrical Performance Summary
The design is based on a wound roll of aluminum foil electrodes separated by paper dielectric, with a target capacitance of 10 µF and a foil width of 0.3 meters. The required physical dimensions change significantly based on the paper thickness and the composite dielectric constant (εᵣ ≈ 3.2) of the oil-impregnated paper.

| Parameter | Using 30 g/m² Paper (~40 µm thick) | Using 60 g/m² Paper (~75 µm thick) | Notes |
| :--- | :--- | :--- | :--- |
| **Foil Length Required** | ~47 meters | ~88 meters | For one of the two foil strips. |
| **Approx. Outer Diameter** | ~6.3 cm | ~11.2 cm | Estimated from winding volume around a 1 cm core. |
| **Estimated Safe DC Voltage** | **300 - 400 V** | **550 - 750 V** | **Critical:** This is a *practical safe voltage*, estimated at 20-30% of the material's theoretical limit to account for imperfections. |
| **Material Breakdown Strength** | Up to **30 kV/mm** (theoretical) | | Conservative estimate for well-impregnated, dry paper-oil composite. The oil displaces air and can increase breakdown strength significantly. |

###  Preparation & Treatment Steps
The process is divided into three main phases, with the CaO treatment being a highly experimental and challenging addition.

1.  **Core Construction & Drying**
    *   **Wind the Capacitor:** Assemble the dry, unimpregnated roll using the specified lengths of aluminum foil and paper.
    *   **Condition the Roll:** Bake the entire wound roll in an oven at 105-110°C for several hours to drive out absorbed moisture. Cool it in a dry environment.

2.  **Oil Treatment with CaO (Experimental & High-Risk)**
    *   **Purpose:** To chemically bind residual water in the olive oil (**CaO + H₂O → Ca(OH)₂**) and neutralize free fatty acids.
    *   **Critical Challenge:** The reaction creates a fine, insoluble **Calcium Hydroxide [Ca(OH)₂]** powder suspended in the viscous oil, which is extremely difficult to remove completely.
    *   **Recommended Separation Method:** **Vacuum Filtration** with a **filter aid** (like diatomaceous earth) is the most effective practical method. Simple decantation will not work, and centrifugation may be insufficient.
    *   **Major Risk:** Any remaining Ca(OH)₂ in the oil can dissociate into conductive ions, severely degrading the dielectric's insulating properties and likely ruining the capacitor.

3.  **Final Impregnation & Sealing**
    *   Immediately after drying, submerge the capacitor roll in the treated, warm olive oil in a vacuum chamber (or as close to a vacuum as possible) to ensure the oil fully penetrates the paper layers.
    *   Seal the impregnated capacitor in an airtight container to prevent moisture re-absorption.

###  Key Takeaways & Recommendations
*   **Trade-off:** Thicker paper (60 g/m²) allows for a much higher voltage rating but results in a capacitor that is nearly twice as large.
*   **Voltage is Theoretical:** The calculated safe voltages are optimistic estimates. Real-world performance depends entirely on the perfection of your drying, impregnation, and sealing.
*   **CaO is Problematic:** The CaO treatment step introduces significant complexity and risk of failure. For a more reliable project, the recommended alternatives are to use a **non-contact "tea bag" method** with silica gel (desiccant) and calcium carbonate (acid scavenger), or to use a purpose-made dielectric fluid like transformer oil.

Glass pipe: https://webaruhaz.lombik.hu/uvegcsovek?product_id=1785


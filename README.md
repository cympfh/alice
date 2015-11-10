Another Language InferenCE: ALICE
---

ALICE is another Inference Machine of Union of Regular Pattern Language.

## usage

training data

```bash
   wc /tmp/text
200
```

```bash
   head /tmp/text
```

```
Recombinant_JJ galectin-3_NN was_VBD labeled_VBN with_IN iodine-123_NN and_CC in_FW vitro_FW binding_NN assays_NNS were_VBD conducted_VBN to_TO test_NN -LRB-_-LRB- 123_CD -RRB-_-RRB- I-galectin-3_NN ability_NN to_TO bind_VB to_TO ECM_NNP targets_NNS ._.
For_IN in_FW vivo_FW studies_NNS ,_, a_DT rat_NN model_NN of_IN induced-MI_NN was_VBD used_VBN ._.
Animals_NNS were_VBD subjected_VBN to_TO magnetic_JJ resonance_NN and_CC micro-SPETC/micro-CT_NN imaging_NN two_CD -LRB-_-LRB- 2_CD W-MI_NN -RRB-_-RRB- or_CC four_CD -LRB-_-LRB- 4_CD W-MI_NN -RRB-_-RRB- weeks_NNS after_IN MI_NNP ._.
Sham_NNP rats_NNS were_VBD used_VBN as_IN controls_NNS ._.
Pharmacokinetic_JJ ,_, biodistribution_NN ,_, and_CC histological_JJ studies_NNS were_VBD also_RB performed_VBN after_IN intravenous_JJ administration_NN of_IN IG3_NN ._.
11B6_CD was_VBD radiolabelled_JJ with_IN -LRB-_-LRB- 111_CD -RRB-_-RRB- In_IN through_IN CHX-A_NN ″_SYM -_: DTPA_NN chelation_NN ._.
In_FW vivo_FW biodistribution_NN and_CC uptake_NN of_IN -LRB-_-LRB- 111_CD -RRB-_-RRB- In-DTPA-11B6_NN were_VBD measured_VBN until_IN 168_CD h_NN post-injection_NN in_IN NMRI_NNP nude_JJ mice_NNS bearing_VBG subcutaneous_JJ LNCaP_NN xenografts_NNS ._.
The_DT binding_NN specificity_NN to_TO hK2_NN was_VBD evaluated_VBN by_IN both_CC in_FW vivo_FW competitive_JJ binding_NN assays_NNS with_IN excess_JJ non-labelled_JJ 11B6_NN and_CC hK2-negative_JJ DU145_NN xenografts_NNS ._.
SPECT/CT_NN imaging_NN of_IN subcutaneous_JJ and_CC intra-tibial_JJ LNCaP_NN xenografts_NNS was_VBD used_VBN to_TO visualize_VB the_DT tumours_NNS ._.
```

run

```bash
   ./alice /tmp/text
# or
   cat /tmp/text | ./alice
```

```
scan from text:/tmp/text
time=199
#book
<> <NN> <VB> <> <> <> to/TO <> IG3/NN <> <DT> <> <NN> for/IN <> of/IN <> <NN> .
<> <IN> <> <> <IN> the/DT adductor/JJ canal/NN <> <IN> <DT> <> .
<> <NN> <> <> <DT> <> <NN> <NN> , <> <> <NN> <VB> <> cardioprotective/JJ effect/NN <IN> <> .
<> <> <VB> <> in/IN <> incidence/NN <> <> in/IN <> <VB> <NN> .
<> <NN> <> <IN> <NN> <> <JJ> than/IN <> <IN> patients/NN with/IN <> <NN> .
The/DT <> <VB> <IN> <> development/NN <> <> <> <> <> <JJ> <> <> <> <RB> <> for/IN the/DT <> <> <NN> <> <> <> .
<> findings/NN <VB> <> the/DT <> <NN> of/IN <JJ> <> and/CC <> <NN> .
<> <NN> <IN> <> <> <NN> in/IN <> <> <NN> <> <> <JJ> <NN> <> in/IN combination/NN with/IN <> <NN> <> <> <> <NN> <IN> <NN> .
<> <> <NN> <VB> <> <> this/DT <NN> <> risk/NN <> in/IN <> <NN> .
<> , patients/NN <IN> <> <JJ> <NN> <> <> <JJ> <> <VB> <> <NN> and/CC <NN> <> <> <> .
<> <NN> <> <JJ> <NN> of/IN <NN> <> MDD/NN and/CC <> <IN> <JJ> <NN> <> <NN> .
<> of/IN <> <JJ> feelings/NN <> <NN> , <> and/CC <> <> <> <> <IN> <> <> <NN> .
<> <VB> <> with/IN <> <JJ> <> rate/NN of/IN <NN> <> <NN> <> <> in/IN patients/NN <> <JJ> <NN> <NN> .
<> <> <> <> <JJ> <> the/DT <> <NN> <> <> <JJ> <NN> for/IN <NN> <> .
In/IN <NN> <> , <> <> <NN> <> <> than/IN <> <NN> <NN> .
#pool
Stent/JJ migration/NN was/VB frequent/JJ ./.
It/PR is/VB the/DT first/JJ valid/JJ and/CC reliable/JJ dementia/NN screening/NN instrument/NN for/IN Chinese/JJ adults/NN with/IN ID/NN ./.
Injection/NN of/IN 15/CD ml/NN dye/VB was/VB sufficient/JJ to/TO spread/VB throughout/IN the/DT adductor/JJ canal/NN and/CC beyond/IN both/DT proximally/RB and/CC distally/RB ./.
Distinct/JJ ultrasonographic/JJ features/NN could/MD be/VB identified/VB separating/VB a/DT subsartorial/JJ injection/NN from/IN an/DT injection/NN within/IN the/DT adductor/JJ canal/NN with/IN consequent/JJ differences/NN in/IN the/DT spread/NN ./.
In/IN the/DT present/JJ cohort/NN of/IN dialysis/NN patients/NN ,/, sKlotho/NN was/VB not/RB independently/RB associated/VB with/IN CVD/NN ./.
Our/PR results/NN suggest/VB that/IN short-term/JJ exposure/NN to/TO particulate/JJ air/NN pollution/NN is/VB associated/VB with/IN increased/VB IHD/NN mortality/NN ./.
Joint/NN hypermobility/NN that/WD is/VB much/RB more/RB common/JJ appears/VB ,/, however/RB ,/, to/TO be/VB a/DT poor/JJ predictor/NN ./.
There/EX was/VB a/DT considerable/JJ decline/NN in/IN mortality/NN rate/NN across/IN almost/RB all/DT age/NN groups/NN ,/, especially/RB in/IN recent/JJ years/NN ./.
However/RB ,/, women/NN over/IN the/DT age/NN of/IN 65/CD years/NN had/VB poorer/JJ outcome/NN throughout/IN the/DT study/NN period/NN ./.
In/IN ready-to-wean/JJ subjects/NN with/IN COPD/NN exacerbations/NN ,/, hyperoxia/NN is/VB followed/VB by/IN an/DT increase/NN in/IN PaCO2/NN ,/, but/CC it/PR does/VB not/RB significantly/RB modify/VB the/DT respiratory/JJ drive/NN or/CC the/DT ventilatory/JJ response/NN to/TO hypercapnia/NN ./.
Incorporating/VB service/NN provider/NN perspectives/NN and/CC scientific/JJ evidence/NN into/IN health/NN education/NN initiatives/NN enhances/VB potential/NN for/IN successful/JJ implementation/NN ,/, particularly/RB when/WR supplemented/VB by/IN ongoing/JJ training/NN and/CC facilitation/NN ./.
However/RB ,/, patients/NN with/IN a/DT low/JJ sKlotho/NN level/NN -LRB-/-L </JJ 460/CD pg/mL/NN -RRB-/-R did/VB show/VB CAD/NN and/CC LV/NN dysfunction/NN more/RB frequently/RB ./.
These/DT findings/NN suggest/VB that/IN men/NN with/IN cT2c/NN disease/NN should/MD be/VB considered/VB to/TO be/VB at/IN intermediate/JJ risk/NN ./.
Therefore/RB ,/, while/IN sKlotho/NN might/MD be/VB a/DT marker/NN for/IN CVD/NN in/IN dialysis/NN patients/NN ,/, the/DT current/JJ data/NN does/VB not/RB support/VB a/DT direct/JJ cardioprotective/JJ effect/NN of/IN sKlotho/NN ./.
Nearly/RB two-thirds/NN of/IN infants/NN experience/VB a/DT rise/NN in/IN serum/NN bilirubin/NN following/VB PN/NN cessation/NN that/WD can/MD last/VB for/IN weeks/NN ,/, but/CC cholestasis/NN generally/RB improves/VB with/IN time/NN in/IN the/DT majority/NN of/IN infants/NN ./.
To/T
```

`book` is the list of learned (Regular Pattern with class var),
and `pool` is the texts not yet learned.


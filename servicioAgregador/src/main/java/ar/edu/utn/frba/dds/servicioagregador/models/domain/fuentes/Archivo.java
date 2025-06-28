package ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
@Builder
public class Archivo {
  private String nombre;
  private Integer tamanioEnBytes;
}

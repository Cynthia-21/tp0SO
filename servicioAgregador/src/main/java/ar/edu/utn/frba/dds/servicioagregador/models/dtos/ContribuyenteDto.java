package ar.edu.utn.frba.dds.servicioagregador.models.dtos;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
@Builder
public class ContribuyenteDto {
  private String nombre;
  private String apellido;
  // fecha de nacimiento
}
